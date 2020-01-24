/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : test.cpp
#   Last Modified : 2019-12-17 12:45
#   Describe      :
#
# ====================================================*/
#include "kth.h"
#include <cstdio>
#include <cstdlib>
typedef long long int ll;
using namespace std;
const int N = 500010;
const int K = 2000003;

inline int min(int a, int b) { return a < b ? a : b; }
inline int max(int a, int b) { return a > b ? a : b; }

int list[3][N];   //三个数组
int cur_list = 0; //目前处理的数组

struct Node {
  int x, y, z;
} heap_ans[2 * K + 2]; //(x,y,z)三元组的堆

int ans_size = 0; //堆的大小

//同一数组id为a_id的数与id为b_id的数进行比较
bool a_is_small(int a_id, int b_id)
{
  bool flag = 0;
  switch (cur_list) { //目前排序数组
  case 0:
    flag = compare(a_id, 1, 1, b_id, 1, 1);
    break;
  case 1:
    flag = compare(1, a_id, 1, 1, b_id, 1);
    break;
  case 2:
    flag = compare(1, 1, a_id, 1, 1, b_id);
    break;
  }
  return flag;
}

//将三元组a加入堆
void heap_add(Node a)
{
  int cur = 1 + ans_size; //加在堆的末尾
  ans_size++;
  heap_ans[cur] = a;
  int parent = cur >> 1;
  while (parent) { // a上浮
    Node p = heap_ans[parent];
    Node c = heap_ans[cur];
    if (compare(list[0][c.x], list[1][c.y], list[2][c.z], list[0][p.x], list[1][p.y], list[2][p.z])) { // a比父亲小就交换
      heap_ans[parent] = c;
      heap_ans[cur] = p;
    } else {
      break;
    }
    cur = parent;
    parent >>= 1;
  }
}

//移除堆顶元素
void remove()
{
  int cur = 1;
  int lc = 2, rc = 3;
  heap_ans[1] = heap_ans[ans_size--]; //将最后的元素放到堆顶
  while (1) {                         //堆顶下沉
    Node p = heap_ans[cur];
    if (lc <= ans_size) { //若左孩子存在
      int target = lc;
      Node c = heap_ans[lc];
      if (rc <= ans_size) { //若右孩子存在
        Node cr = heap_ans[rc];
        if (compare(list[0][cr.x], list[1][cr.y], list[2][cr.z], list[0][c.x], list[1][c.y], list[2][c.z])) { // 取较小的孩子
          c = cr;
          target = rc;
        }
      }
      if (compare(list[0][c.x], list[1][c.y], list[2][c.z], list[0][p.x], list[1][p.y], list[2][p.z])) { //若比较小的孩子大，交换
        heap_ans[cur] = c;
        heap_ans[target] = p;
        cur = target;
        lc = target << 1;
        rc = lc + 1;
        continue;
      }
    }
    break;
  }
}

//同一数组中，id为p与id为q的两数进行比较
int cmp(const void* p, const void* q)
{
  int x = *(const int*)p;
  int y = *(const int*)q;
  if (a_is_small(x, y))
    return -1; //升序
  else
    return 1;
}

void get_kth(int n, int k, int* x, int* y, int* z)
{
  for (int i = 0; i < 3; i++) { //初始化三个数组，存放id
    for (int j = 1; j <= n; j++) {
      list[i][j] = j;
    }
    cur_list = i;
    //对三个数组，按值对id排序，值较小的id在前
    qsort(list[i] + 1, n, sizeof *list[i], cmp);
  }
  Node cur;
  Node temp = cur;
  cur.x = cur.y = cur.z = 1; //(1,1,1)一定是最小的
  heap_add(cur);             //(1,1,1)进堆
  int num = 1;
  while (1) {
    cur = heap_ans[1]; //当前结果最小的三元组(x,y,z)

    if (num == k) { //输出第k小的三元组
      *x = list[0][cur.x];
      *y = list[1][cur.y];
      *z = list[2][cur.z];
      return;
    }

    temp = cur;
    temp.z++; //扩展至(x,y,z+1)
    if (temp.z <= n)
      heap_add(temp);

    temp = cur;
    if (temp.z == 1) {
      temp.x++; //若在xy平面，扩展至(x+1,y,1)
      if (temp.x <= n)
        heap_add(temp);

      temp = cur;
      if (temp.x == 1) { //若在x轴，扩展至(1,y+1,1)
        temp.y++;
        if (temp.y <= n)
          heap_add(temp);
      }
    }

    remove(); //此时堆顶的必为第num小的三元组，移除
    num++;    //移除后堆顶的为第num+1小的
  }
}
