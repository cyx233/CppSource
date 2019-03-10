#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"func.h"


void Poly(int argc, char*argv[])
{
    double sum = 0, x = 0;
    x =  static_cast<double>(atof(argv[argc-1]));
    sum =  static_cast<double>(atof(argv[1]));
    for(int i = 2; i < argc-1; i++)
    {
        double n =  static_cast<double>(atof(argv[i]));
        sum += n * pow(x,i-1);
    }
    return;
}