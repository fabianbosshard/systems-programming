CC     = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

BIN  = test_path
OBJS = path.o

.PHONY: all clean run

all: $(BIN)

$(BIN): test_path.c $(OBJS) path.h
	$(CC) $(CFLAGS) -o $@ test_path.c $(OBJS)

path.o: path.c path.h
	$(CC) $(CFLAGS) -c path.c -o $@

run: $(BIN)
	./$(BIN)

clean:
	rm -f $(BIN) $(OBJS)