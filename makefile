krim: main.s
	as main.s -o main.o
	ld -e krim main.o -o krim
	rm main.o
