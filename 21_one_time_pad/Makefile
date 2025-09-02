# Makefile for Exercise 21: otp
# Builds otp from otp.c and runs tests.
SHELL := /bin/bash
CC    := gcc
CFLAGS:= -std=c11 -Wall -Wextra -O2
SRC   := otp.c
BIN   := otp

BDIR  := build
K1    := $(BDIR)/key1
K1B   := $(BDIR)/key1b
K2    := $(BDIR)/key2

.PHONY: all test clean

all: $(BIN)

$(BIN): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

test: $(BIN)
	@set -euo pipefail; \
	mkdir -p $(BDIR); \
	echo "==> Generating keys"; \
	dd if=/dev/urandom of=$(K1) bs=1 count=4096 status=none; \
	cp $(K1) $(K1B); \
	dd if=/dev/urandom of=$(K2) bs=1 count=4096 status=none; \
	rm -f $(K1).counter $(K1B).counter $(K2).counter; \
	printf "0" > $(K1).counter; \
	printf "0" > $(K1B).counter; \
	printf "0" > $(K2).counter; \
	\
	echo "==> T0: pipeline roundtrip (stdin/stdout) with identical keystreams"; \
	plain0="The quick brown fox jumps over 13 lazy dogs."; \
	out0="$$(echo -n "$$plain0" | ./$(BIN) key=$(K1) | ./$(BIN) key=$(K1B))"; \
	if [[ "$$out0" != "$$plain0" ]]; then echo "T0 FAIL"; exit 1; fi; \
	echo "T0 OK"; \
	\
	echo "==> T1: file I/O roundtrip; decrypt with a key COPY"; \
	echo -n "hello otp world" > $(BDIR)/m1.txt; \
	./$(BIN) in=$(BDIR)/m1.txt out=$(BDIR)/c1.bin key=$(K1); \
	# Decrypt using an identical key copy starting at 0
	rm -f $(K1B) $(K1B).counter; cp $(K1) $(K1B); printf "0" > $(K1B).counter; \
	./$(BIN) in=$(BDIR)/c1.bin out=$(BDIR)/p1.txt key=$(K1B); \
	diff -q $(BDIR)/m1.txt $(BDIR)/p1.txt >/dev/null; \
	[[ -f $(K1).counter && -f $(K1B).counter ]]; \
	echo "T1 OK"; \
	\
	echo "==> T2: two messages advance the same key's counter correctly"; \
	printf "ABCDEF" > $(BDIR)/m2a.txt; \
	printf "0123456789" > $(BDIR)/m2b.txt; \
	K3=$(BDIR)/key3; rm -f $$K3 $$K3.counter; dd if=/dev/urandom of=$$K3 bs=1 count=4096 status=none; printf "0" > $$K3.counter; \
	./$(BIN) in=$(BDIR)/m2a.txt out=$(BDIR)/c2a.bin key=$$K3; \
	./$(BIN) in=$(BDIR)/m2b.txt out=$(BDIR)/c2b.bin key=$$K3; \
	len_a=$$(wc -c < $(BDIR)/m2a.txt); \
	len_b=$$(wc -c < $(BDIR)/m2b.txt); \
	counter_val=$$(cat $$K3.counter); \
	expected=$$((len_a + len_b)); \
	if [[ "$$counter_val" != "$$expected" ]]; then \
	  echo "T2 FAIL: counter=$$counter_val expected=$$expected"; exit 1; \
	fi; \
	echo "T2 OK"; \
	\
	echo "==> T3: defaults (stdin/stdout, default key name 'key')"; \
	rm -f key key.counter; \
	dd if=/dev/urandom of=key bs=1 count=2048 status=none; \
	echo -n "minimal default path" | ./$(BIN) > $(BDIR)/c3.bin; \
	[[ -f key.counter ]]; \
	rm -f key2 key2.counter; cp key key2; printf "0" > key2.counter; \
	cat $(BDIR)/c3.bin | ./$(BIN) key=key2 > $(BDIR)/p3.txt; \
	echo -n "minimal default path" > $(BDIR)/m3.txt; \
	diff -q $(BDIR)/m3.txt $(BDIR)/p3.txt >/dev/null; \
	echo "T3 OK"; \
	\
	echo "==> All tests PASSED."

clean:
	rm -rf $(BDIR) $(BIN) key key.counter key2 key2.counter