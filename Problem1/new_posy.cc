#include<stdio.h>
#include<cstdlib>
#include<cmath>
#include"func.h"

double NewPosy(int argc, char*argv[]){
    double sum = 0, x = 0;
    x = static_cast<double>(atof(argv[argc-1]));
    if (x<1E-6) {
        printf("Error,x = 0\n");
    }
    
    sum = static_cast<double>(atof(argv[argc-2]))/x + static_cast<double>(atof(argv[argc-3]));

    for(int i = argc - 4; i >= 1; i--){
        double n = static_cast<double>(atof(argv[i]));
        sum = n + sum/x;
    }
    return sum;
}