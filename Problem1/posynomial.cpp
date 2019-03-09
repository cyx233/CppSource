#include<cstdio>
#include<cstdlib>
#include<cmath>
#include"func.h"
using namespace std;


void posy(int argc, char*argv[])
{
    double sum = 0, x = 0;
    x = (double)atof(argv[argc-1]);
    if (x<1E-6) {
        printf("Error,x = 0\n");
        return;
    }
    
    sum = (double)atof(argv[1]);
    for(int i = 2; i < argc-1; i++)
    {
        double n = (double)atof(argv[i]);
        sum += n/pow(x,i-1);
    }
    
    printf("the value of posynomial is %.2lf",sum);

    return;
}