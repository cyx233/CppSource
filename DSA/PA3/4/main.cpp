/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : main.cpp
#   Last Modified : 2019-12-08 16:10
#   Describe      :
#
# ====================================================*/
#include <cstdio>
int kmp[20000010]; // kmp数组
int dp[20000010];  //动态规划数组
char s[20000010];  //输入
int main()
{
  int len = 1, j = 0;   // len为当前位置
  kmp[0] = kmp[1] = 0;  //初始化kmp
  s[0] = getchar();     //至少一个字符
  while (1) {           // kmp算法
    s[len] = getchar(); //读入数据
    if (s[len] < 'a' || s[len] > 'z')
      break;        //结束读入
    j = kmp[len++]; //[0,j)为与后缀能匹配的最长前缀
    while (j && s[len - 1] != s[j])
      j = kmp[j];                              //若不能匹配[0,j]，则尝试匹配[0,j)的前缀
    kmp[len] = s[len - 1] == s[j] ? j + 1 : 0; //若找到匹配前缀，前缀长度+1，否则重置为0
  }
  long long int count = 0;         //答案为最大为4E14，使用long long int
  for (int i = 1; i <= len; i++) { //动态规划每一个位置
    dp[i] = dp[kmp[i]] + 1;        //子串仍有其匹配前缀，此位置计数需要考虑嵌套的匹配前缀
    count += dp[i];                //此位置应被计数的次数计入总和
  }
  printf("%lld\n", count); //输出答案
}
