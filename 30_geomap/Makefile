CC      ?= cc          # for C sources (test_geomap.c)
CXX     ?= g++         # or clang++
CFLAGS  ?= -Wall
CXXFLAGS?= -std=c++11 -Wall

.PHONY: all clean test very-clean
all: test

geomap.o: geomap.cc geomap.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

test_geomap.o: test_geomap.c geomap.h
	$(CC) $(CFLAGS) -c $< -o $@

test_geomap: test_geomap.o geomap.o
	$(CXX) $(CXXFLAGS) $^ -o $@

test: test_geomap
	./test_geomap

clean:
	rm -f geomap.o test_geomap.o

very-clean: clean
	rm -f test_geomap