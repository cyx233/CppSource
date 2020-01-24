/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : main.cpp
#   Last Modified : 2019-12-07 21:50
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
const int DEFAULT_CAPACITY = 2;

inline int min(int a, int b) { return a < b ? a : b; }
inline int max(int a, int b) { return a > b ? a : b; }

//课件上的Vector模板类
template <typename T> class Vector {
  private:
  int _size;
  int _capacity;
  T* _elem;

  protected:
  void copyFrom(T const* A, int lo, int hi)
  { //拷贝
    _elem = new T[_capacity = 2 * (hi - lo)];
    _size = 0;
    while (lo < hi)
      _elem[_size++] = A[lo++];
  }
  void expand()
  { //扩容
    if (_size < _capacity)
      return;
    _capacity = max(_capacity, DEFAULT_CAPACITY);
    _capacity = _capacity << 1;
    T* oldElem = _elem;
    _elem = new T[_capacity];
    for (int i = 0; i < _size; i++)
      _elem[i] = oldElem[i];
    delete[] oldElem;
  }

  void shrink()
  { //缩减
    if (_size + 1 > _capacity >> 2)
      return;
    _capacity = max(_capacity, DEFAULT_CAPACITY << 1);
    _capacity = _capacity >> 1;
    T* oldElem = _elem;
    _elem = new T[_capacity];
    for (int i = 0; i < _size; i++)
      _elem[i] = oldElem[i];
    delete[] oldElem;
  }

  public:
  //默认构造
  Vector(int c = DEFAULT_CAPACITY)
  {
    _elem = new T[_capacity = c];
    _size = 0;
  }
  //数组拷贝构造
  Vector(T const& A, int lo, int hi) { copyFrom(A, lo, hi); }
  //部分拷贝构造
  Vector(Vector<T> const& V, int lo, int hi) { copyFrom(V._elem, lo, hi); }
  //拷贝构造
  Vector(Vector<T> const& V) { copyFrom(V._elem, 0, V._size); }
  ~Vector() { delete[] _elem; }

  //将元素e插入到第r个
  int insert(int r, T const& e)
  {
    expand();
    for (int i = _size; i > r; i--) {
      _elem[i] = _elem[i - 1];
    }
    _elem[r] = e;
    _size++;
    return r;
  }
  //移除[lo, hi)
  int remove(int lo, int hi)
  {
    if (lo == hi)
      return 0;
    while (hi < _size)
      _elem[lo++] = _elem[hi++];
    _size = lo;
    shrink();
    return hi - lo;
  }
  //移除第r个
  T remove(int r)
  {
    T e = _elem[r];
    remove(r, r + 1);
    return e;
  }
  //在末尾插入
  int push_back(T const& e) { return insert(_size, e); }
  //重载[]
  T& operator[](int r) { return _elem[r]; }
  //返回size
  int size() { return _size; }
};

int list[3][N];   //三个数组
int cur_list = 0; //目前处理的数组

struct Node {
  int x, y, z;
} heap_ans[2 * K + 2]; //(x,y,z)三元组的堆

int ans_size = 0; //堆的大小

Vector<Node> dic[K]; //三元组字典

bool set(Node a)
{                                                                    //将三元组a加入字典dic
  int cur = ((ll)a.x + (ll)a.y * 500009 + (ll)a.z * 500029) % (ll)K; // hash函数
  for (int i = 0; i < dic[cur].size(); i++) {                        //去重
    if (dic[cur][i].x == a.x && dic[cur][i].y == a.y && dic[cur][i].z == a.z)
      return 0;
  }
  dic[cur].push_back(a); //加入字典
  return 1;
}

//同一数组id为a_id的数与id为b_id的数进行比较
bool a_is_small(int a_id, int b_id)
{
  bool flag = 0;
  switch (cur_list) {
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
  set(cur);                  //(1,1,1)进堆
  heap_add(cur);             //(1,1,1)加入字典
  int num = 1;
  while (1) {
    cur = heap_ans[1]; //当前结果最小的三元组

    if (num == k) { //输出第k小的三元组
      *x = list[0][cur.x];
      *y = list[1][cur.y];
      *z = list[2][cur.z];
      return;
    }

    temp = cur; //若当前最小三元组为(x,y,z)
    temp.x++;   //考虑(x+1,y,z)
    if (temp.x <= n)
      if (set(temp)) {  //若第一次出现
        heap_add(temp); //将(x+1,y,z)加入堆
      }

    temp = cur;
    temp.y++; //考虑(x,y+1,z)
    if (temp.y <= n)
      if (set(temp)) {  //若第一次出现
        heap_add(temp); //将(x,y+1,z)加入堆
      }

    temp = cur;
    temp.z++; //考虑(x,y,z+1)
    if (temp.z <= n)
      if (set(temp)) {  //若第一次出现
        heap_add(temp); //将(x,y,z+1)加入堆
      }

    remove(); //此时堆顶的必为第num小的三元组，移除
    num++;    //移除后堆顶的为第num+1小的
  }
}
