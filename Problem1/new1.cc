#include<cstdio>
#include"func.h"

namespace std{

  int main(int argc, char* argv[]){
    for(int i=1; i<=10000)
      for(int j=1; j<=10000)
        NewPoly(argc, argv);
    return 0;
  }
}