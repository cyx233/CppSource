#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"func.h"
using namespace std;

double NewPoly(int argc, char*argv[]){

    double sum = 0, x = 0;
    
    x = static_cast<double>(atof(argv[argc-1]));
    sum = static_cast<double>(atof(argv[argc-2]))*x + static_cast<double>(atof(argv[argc-3]));

    for(int i = argc - 4; i >= 1; i--){
        double n = static_cast<double>(atof(argv[i]));
        sum = n + x*sum;
    }
    return sum;
}