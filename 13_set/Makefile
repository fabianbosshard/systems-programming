# Minimal makefile to build + run the tests

CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -g
LDFLAGS ?=
TARGET   = test_cset
OBJS     = cset.o test_cset.o

.PHONY: all test run clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

cset.o: cset.c cset.h
	$(CC) $(CFLAGS) -c $<

test_cset.o: test_cset.c cset.h
	$(CC) $(CFLAGS) -c $<

# Build then execute the test binary
test: $(TARGET)
	./$(TARGET)

run: test

clean:
	rm -f $(TARGET) *.o