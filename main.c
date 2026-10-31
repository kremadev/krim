#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>

struct termios orig;
struct winsize ws;

void move(int x, int y) {
	char buf[64];
	int l = snprintf(buf, 64, "\x1b[%d;%dH", y+1, x+1);
	write(1, buf, l);
}

void disableMode(void) {
	char *leave = "\x1b[?1049l";
	write(STDOUT_FILENO, leave, strlen(leave));
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

void enableMode(void) {
	tcgetattr(STDIN_FILENO, &orig);
	atexit(disableMode);
	struct termios raw = orig;
	raw.c_lflag &= ~(ECHO | ICANON);
	raw.c_cc[VMIN] = 1;
	raw.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
	char *enter = "\x1b[?1049h";
	write(STDOUT_FILENO, enter, strlen(enter));
	ioctl(1, TIOCGWINSZ, &ws);
}

int main(int ac, char **av) {
	enableMode();
	char lines[2048][1024] = {0};
	int line_count = 0;
	char in[32];
	int quit = 0;
	int cx = 0;
	int cy = 0;

	while (!quit) {
		read(0, in, 1);
		switch (in[0]) {
			case 27:
				read(0, in, 1);
				if (in[0] == '[') {
					read(0, in, 1);
					switch (in[0]) {
						case 'C':
							if (cx != strlen((const char *)lines[cy])) {
								cx++;
							} else if (cy != line_count) {
								cx = 0;
								cy++;
							} break;
						case 'D':
							if (cx != 0) {
								cx--;
							} else if (cy != 0) {
								cy--;
								cx = strlen(lines[cy]);
							} break;
					}
				} else if (in[0] == 'q') {
					quit = 1;
				} break;
			case 10:
				if (cy == line_count) {
					cy++;
					line_count++;
					cx = 0;
				} break;
			case 127:
				if (cx != 0 && cx == strlen(lines[cy])) {
					cx--;
					lines[cy][cx] = 0;
					write(1, "\b \b", 3);
				} else if (cx == 0 && cy > 0 && strlen(lines[cy]) == 0 && cy == line_count) {
					line_count--;
					cy--;
					cx = strlen(lines[cy]);
					lines[cy][cx] = 0;
				} break;
			default:
				lines[cy][cx] = in[0];
				write(1, in, 1);
				cx++;
		}
		move(0, ws.ws_row-1);
		char statusbar[ws.ws_col];
		int n = snprintf(statusbar, ws.ws_col, " -- %s --", av[1]);
		write(1, statusbar, n);
		move(cx, cy);
	}
	FILE *file = fopen(av[1], "w");

	for (int i = 0; i <= line_count; i++) {
		fwrite(lines[i], 1, strlen(lines[i]), file);
		fputc('\n', file);
	}
	fclose(file);

	return 0;
}
