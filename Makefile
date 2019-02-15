CC=gcc
CFLAGS=-g -Wall -pedantic -std=c99
LFLAGS=-lm

jungle.x86_64: build/family.o build/field.o build/individual.o \
	build/main.o build/simulation.o build/simulation.o build/tools.o
	${CC} ${LFLAGS} $^ -o $@

build/%.o: src/%.c
	$(CC) -c $(CFLAGS) $^ -o $@

clean:
	rm build/*
