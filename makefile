main.out: main.o city.o city.h
	g++ -g main.o city.o -o main.out
main.o: main.cc city.h
	g++ -c -g main.cc
city.o: city.cc city.h
	g++ -c -g city.cc