CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra

all: ROMreader trace_comparison

trace_comparison: trace_comparison.cpp
	$(CXX) $(CFLAGS) -o $@ $<

ROMreader: ROMreader.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o ROMreader trace_comparison

.PHONY: all clean