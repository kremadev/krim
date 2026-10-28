#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct termios orig;

void disableMode(void) {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig);
}

void enableMode(void) {
	tcgetattr(STDIN_FILENO, &orig);
	atexit(disableMode);
	struct termios raw = orig;
	raw.c_lflag &= ~(ECHO | ICANON);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(int ac, char **av) {
	enableMode();

	disableMode();
	return 0;
}
