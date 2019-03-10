#include<cstdio>
#include"func.h"
#include<time.h>
#include<cmath>
#define TEST 1

int main(int argc,char*argv[]){


  clock_t start, end;

//old poly
  start = clock();

  for(int i=1; i<=TEST; i++)
    for(int j=1; j<=TEST; j++)
      Poly(argc,argv);
  end=clock();

  double time_old_poly = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

//old posy
  start = clock();

  for(int i=1; i<=TEST; i++)
    for(int j=1; j<=TEST; j++)
      Posy(argc,argv);
  end=clock();

  double time_old_posy = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

  //new poly
  start = clock();

  for(int i=1; i<=TEST; i++)
    for(int j=1; j<=TEST; j++)
      NewPoly(argc,argv);
  end=clock();

  double time_new_poly = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

  //new posy
  start = clock();

  for(int i=1; i<=TEST; i++)
    for(int j=1; j<=TEST; j++)
      NewPosy(argc,argv);
  end=clock();

  double time_new_posy = (end-start)/static_cast<double>(CLOCKS_PER_SEC);

  print(time_old_poly,time_new_poly,time_old_posy,time_new_posy,
        NewPoly(argc,argv),NewPosy(argc,argv));
}