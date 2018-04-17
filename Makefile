CFLAGS= -g -Wall -O3 -std=gnu11
LDLIBS=
LDFLAGS=
CC=gcc

SRC=./src
BIN=./bin


$(BIN)/$(P): $(SRC)/$(P).c
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@
