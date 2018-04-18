CFLAGS= -g -Wall -std=gnu11
LDLIBS=
LDFLAGS=
CC=gcc

SRC=./src
BIN=./bin


$(BIN)/$(P): $(SRC)/$(P).c
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(LDLIBS) -o $@

clean:
	rm $(BIN)/*
