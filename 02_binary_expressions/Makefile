# Makefile for bexp tests
# Expects: bexp.c and bexp.h in the same directory.

CC      ?= cc
CFLAGS  ?= -std=c99 -Wall -Wextra -pedantic -O2
LDFLAGS ?=
TARGET  := test_bexp
SRCS    := test_bexp.c bexp.c
OBJS    := $(SRCS:.c=.o)

.PHONY: all clean run test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

%.o: %.c bexp.h
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: run

clean:
	$(RM) $(OBJS) $(TARGET)
	