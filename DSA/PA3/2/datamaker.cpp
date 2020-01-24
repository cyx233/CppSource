/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : datamaker.cpp
#   Last Modified : 2019-12-10 21:11
#   Describe      :
#
# ====================================================*/

#include "crc32.h"
#include <cstdlib>
#include <iostream>
#include <time.h>
using namespace std;

const int N = 1000000;
const char alpha[25] = "0123456789tsinghua";

int main()
{
  printf("%d\n", N);
  srand(time(NULL));
  char salt[5];
  salt[4] = '\0';
  for (int i = 0; i < 4; i++) {
    salt[i] = alpha[rand() % 18];
  }
  printf("%s\n", salt);
  char temp[10];
  for (int i = 0; i < N; i++) {
    int len = rand() % 6 + 1;
    temp[len] = '\0';
    for (int j = 0; j < len; j++) {
      temp[j] = alpha[rand() % 18];
    }
    unsigned raw = crc32(0, (unsigned char*)temp, len);
    unsigned result = crc32(raw, (unsigned char*)salt, 4);
    printf("%x\n", result);
  }
  return 0;
}
