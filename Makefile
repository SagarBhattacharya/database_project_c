CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c17 -g
SRCDIR = src

SRCS = $(wildcard $(SRCDIR)/*.c)

all: xdb

xdb : $(SRCS)
	$(CC) $^ -o $@ $(CFLAGS)

clean:
	del *.exe

run: xdb
	.\xdb.exe

PHONY: all clean run