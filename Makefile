BIN="RadioclockSDL"
CC="gcc"

default:
	${CC} --std=c99 -o ${BIN} primitives.c main.c -lSDL -lSDLmain -lm

clean:
	rm -f ./RadioclockSDL
