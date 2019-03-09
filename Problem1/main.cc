#include"func.h"
#include<cstdio>
#include<time.h>

namespace std{
  int main(int argc,char*argv[]){

    double poly_sum = NewPoly(argc,argv);
    double posy_sum = NewPosy(argc,argv);
    
    
    __clock_t start, end;

    start = clock();

    for (int i = 0; i < 10000; i++)
      for (int j = 0; j < 10000; j++)
          Poly(argc,argv);

    end=clock();
    
    double runtime_old_poly = static_cast<double>((end - start)/CLOCKS_PER_SEC);

    //////////////
    start = clock();

    for (int i = 0; i < 10000; i++)
      for (int j = 0; j < 10000; j++)
          Posy(argc,argv);

    end=clock();
    
    double runtime_old_posy = static_cast<double>((end - start)/CLOCKS_PER_SEC);

    //////////////
    start = clock();

    for (int i = 0; i < 10000; i++)
      for (int j = 0; j < 10000; j++)
          NewPoly(argc,argv);

    end=clock();
    
    double runtime_new_poly = static_cast<double>((end - start)/CLOCKS_PER_SEC);

    //////////////
    start = clock();

    for (int i = 0; i < 10000; i++)
      for (int j = 0; j < 10000; j++)
          NewPosy(argc,argv);

    end=clock();
    
    double runtime_new_posy = static_cast<double>((end - start)/CLOCKS_PER_SEC);

    printf("poly sum %lf, posy sum:%lf\n",poly_sum,posy_sum);
    printf("Total runtime of old method is posy:%lfs, poly:%lfs\n",runtime_old_posy,runtime_old_poly);
    printf("Total runtime of Qing method is posy:%lfs, poly:%lfs\n",runtime_new_posy,runtime_new_poly);
    
    return 0;
  }
}   