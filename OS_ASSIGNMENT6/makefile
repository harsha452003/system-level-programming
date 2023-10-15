CC=g++
CFLAGS=-Wall

all: libgoodmalloc.a mergesort

libgoodmalloc.a: goodmalloc.o
	ar -rcs libgoodmalloc.a goodmalloc.o

goodmalloc.o: goodmalloc.cpp goodmalloc.hpp
	$(CC) $(CFLAGS) -c goodmalloc.cpp

mergesort: mergesort.o libgoodmalloc.a
	$(CC) $(CFLAGS) -o mergesort mergesort.o -L. -lgoodmalloc

mergesort.o: mergesort.cpp
	$(CC) $(CFLAGS) -c mergesort.cpp

clean:
	rm -f *.o *.a mergesort memory_footprint.txt