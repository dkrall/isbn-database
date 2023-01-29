all: main

main: main.o Product.o ProductCategory.o
	g++ -std=c++14 -o main main.o Product.o ProductCategory.o

main.o: main.cpp
	g++ -std=c++14 -c main.cpp

Product.o: Product.cpp Product.h
	g++ -std=c++14 -c Product.cpp

ProductCategory.o: ProductCategory.cpp ProductCategory.h
	g++ -std=c++14 -c ProductCategory.cpp

clean:
	rm *.o
