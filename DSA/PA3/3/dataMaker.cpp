/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : dataMaker.cpp
#   Last Modified : 2019-12-17 14:53
#   Describe      :
#
# ====================================================*/

#include <cstdio>
#include <cstdlib>
#include <ctime>
using namespace std;

/* const int n = 500000; */
/* const int k = 2000000; */
const int n = 500000;
const int k = 2000000;
int main()
{
  srand(time(NULL));
  printf("%d %d\n", n, k);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < n; j++) {
      printf("%d ", rand());
    }
    printf("\n");
  }
  return 0;
}
