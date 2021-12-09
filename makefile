CC=g++

all:
	$(CC) -o test test.cpp

clean:
	rm -f ./test