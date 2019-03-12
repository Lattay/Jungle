CC=gcc
CFLAGS=-g -Wall -Wextra -pedantic -std=c99
LFLAGS=-lm

all: jungle.x86_64

run: jungle.x86_64
	./jungle.x86_64 out.bin

test: test.x86_64
	./test.x86_64

jungle.x86_64: build/main.o build/tools.o build/population.o build/field.o \
	           build/simulation.o build/output.o
	${CC} ${LFLAGS} $^ -o $@

test.x86_64: build/test.o build/unity.o build/tools.o build/population.o \
	         build/field.o build/simulation.o
	${CC} ${LFLAGS} $^ -o $@

build/test.o: test/test.c build
	${CC} ${CFLAGS} -c $< -o $@

build/unity.o: test/ext/unity.c build
	${CC} ${CFLAGS} -c $< -o $@

build/main.o: src/main.c src/config.h build
	${CC} ${CFLAGS} -c $< -o $@

build/%.o: src/%.c src/%.h build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir build

clean:
	rm build/*
	rmdir build
