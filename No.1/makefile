main.out: main.o city.o city.h
    g++ main.o city.o -o main.out
main.o: main.cc city.h
    g++ -c main.cc
city.o: city.cc city.h
    g++ -c city.cc
