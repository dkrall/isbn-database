all: main

main: main.o Product.o
	g++ -std=c++14 -o main main.o Product.o

main.o: main.cpp
	g++ -std=c++14 -c main.cpp

Product.o: Product.cpp Product.h
	g++ -std=c++14 -c Product.cpp

clean:
	rm *.o
