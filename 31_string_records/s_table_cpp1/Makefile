CFLAGS=-Wall -g
CXXFLAGS=-std=c++11 -Wall -g
CC=$(CXX)

.PHONY: all
all: test

test_s_table: test_s_table.o s_table.o
	$(CXX) $(CFLAGS) $(LDFLAGS) test_s_table.o s_table.o -o $@

.PHONY: clean
clean:
	rm -f s_table.o test_s_table.o

.PHONY: test
test: test_s_table
	./test_s_table

.PHONY: very-clean
very-clean: clean
	rm -f test_s_table

