#include<cstdio>
#include"func.h"

void print(double opoly,double npoly,double oposy,
           double nposy,double spoly,double sposy)
{
  printf("Runtime of old method is:\n");
  printf("poly:%.2lfs\n",opoly);
  printf("posy:%.2lfs\n",oposy);
  printf("Total:%.2lf\n\n",opoly+oposy);

  printf("Runtime of new method is:\n");
  printf("poly:%.2lfs\n",npoly);
  printf("posy:%.2lfs\n",nposy);
  printf("Total:%.2lf\n\n",npoly+nposy);

  printf("Sum is\n");
  printf("poly:%.2lf posy:%.2lf\n",spoly,sposy);

  printf("imp of poly is %.2lf",1-npoly/opoly);
  printf("imp of posy is %.2lf",1-nposy/oposy);

  return;
}