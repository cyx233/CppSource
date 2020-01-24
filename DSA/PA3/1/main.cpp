/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : main.cpp
#   Last Modified : 2019-12-09 19:32
#   Describe      :
#
# ====================================================*/
#include <cstdio> #include <cstring>
const int N = 1 << 24; //最大输入长度
// M为模拟二叉树的bitmap，input为存储输入的bitmap
char M[(N + 8) >> 2], input[(N + 7) >> 3];
//读入input的第k位
inline void set_input(int k) { input[k >> 3] |= 0x80 >> (k & 7); }
//查询input的第k位
inline bool test_input(int k) { return input[k >> 3] & 0x80 >> (k & 7); }
//二叉树中编号为k的叶子存在
inline void set(int k) { M[k >> 3] |= 0x80 >> (k & 7); }
//查找编号k的叶子是否存在
inline bool test(int k) { return M[k >> 3] & 0x80 >> (k & 7); }

int main()
{
  char c;
  c = getchar();
  int tot = 0;
  while (c == '1' || c == '0') { //保存输入
    if (c - '0')
      set_input(tot);
    tot++;
    c = getchar();
  }
  int cnt = 24 < tot ? 24 : tot; //答案最长不会超过24位
  int current = 1;               //当前的二叉树节点编号
  set(1);                        //根节点1初始化为存在
  int l = 0, r = cnt;
  //初始化滑动窗口current
  for (int i = l; i < r; i++) { //[l,r)位之间的字符串，对应叶子编号为current
    if (test_input(i)) {
      current = (current << 1) + 1;
    } else {
      current = current << 1;
    }
  }
  int temp = current;
  //从叶子向上，新建节点，遇到已存在节点则停(剪枝)
  while (!test(temp)) {
    set(temp);
    temp >>= 1;
  }
  //窗口向右滑动一格
  current &= ~(1 << (r - l));  //前缀1删除
  current |= 1 << (r - l - 1); //最高位变成前缀1
  l++;
  if (r < tot) { //窗口末尾已经到达输入信息的末尾
    r++;
  }
  while (l < r) {              //若窗口不为空
    if (r - l == cnt) {        //若窗口大小不变，说明末尾需要新读入一位
      if (test_input(r - 1)) { //读入最后一位
        current = (current << 1) + 1;
      } else {
        current <<= 1;
      }
    }
    int temp = current;
    while (!test(temp)) { //新的窗口插入树
      set(temp);
      temp >>= 1;
    }
    //窗口向右滑动一格
    current &= ~(1 << (r - l));
    current |= 1 << (r - l - 1);
    l++;
    if (r < tot) {
      r++;
    }
  }
  //层次遍历，找到最左上的不存在的节点，即为字典序最小的不存在节点
  for (int i = 2; i <= (N << 1) + 1; i++) {
    if (!test(i)) {
      current = i;
      break;
    }
  }
  int ans_len = 0;
  int ans[25];
  while (current > 1) { //从找到的节点回溯到根节点
    ans[ans_len++] = current & 1;
    current >>= 1;
  }
  for (int i = ans_len - 1; i >= 0; i--) { //正序输出答案
    printf("%d", ans[i]);
  }
  printf("\n");
  return 0;
}
