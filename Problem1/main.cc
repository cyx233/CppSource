#include"func.h"
#include<stdio.h>
#include <time.h>

namespace std{

    int main(int argc,char*argv[]){
    
    __clock_t start, end;

    start = clock();

    for (int i = 0; i < 10000; i++)
        for (int j = 0; j < 10000; j++)
            poly(argc,argv);
    return 0;

    end=clock();
    
    double runtime_old_poly = static_cast<double>((end - start)/CLOCK_PER_SEC


    }
    
}   