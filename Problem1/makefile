main: func.h main.o polynomial.o posynomial.o
	g++ polynomial.o posynomial.o main.o -o main
main.o: main.cpp func.h
	g++ -c -g main.cpp -o main.o
polynomial.o: polynomial.cpp func.h
	g++ -c -g polynomial.cpp -o polynomial.o
posynomial.o: posynomial.cpp func.h
	g++ -c -g posynomial.cpp -o posynomial.o