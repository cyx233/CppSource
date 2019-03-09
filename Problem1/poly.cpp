#include<stdio.h>
#include<cstdlib>
#include<cmath>
#include"func.h"
using namespace std;


void poly(int argc, char*argv[])
{
    double sum = 0, x = 0;
    x = (double)atof(argv[argc-1]);
    sum = (double)atof(argv[1]);
    for(int i = 2; i < argc-1; i++)
    {
        double n = (double)atof(argv[i]);
        sum += n * pow(x,i-1);
    }
    printf("the value of polynomial is %.2lf\n",sum);
    return;
}