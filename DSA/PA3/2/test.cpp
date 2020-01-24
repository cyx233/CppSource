/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : main.cpp
#   Last Modified : 2019-12-10 13:25
#   Describe      :
#
# ====================================================*/

#include "crc32.h"
#include <cstdio>
#include <cstring>
using namespace std;
typedef long long int ll;

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

  //删除开头
  T pop_front() { return remove(0); }

  //重载[]
  T& operator[](int r) { return _elem[r]; }
  //返回size
  int size() { return _size; }
};

struct Node {
  unsigned key;
  char value[10];
};

const char alpha[25] = "0123456789tsinghua";
char salt[10];
const int BIG_P = 2501003;
Vector<Node> dic[BIG_P + 10];
int n;

int hash(unsigned input)
{
  ll temp = input;
  temp %= BIG_P;
  temp = (temp * 2018011253) % BIG_P;
  return (int)temp;
}

void set(const unsigned result, const char* string)
{
  int temp = hash(result);
  for (int i = 0; i < (int)dic[temp].size(); i++) {
    if (dic[temp][i].key == result) {
      if (strcmp(dic[temp][i].value, string)) {
        strcpy(dic[temp][i].value, "Duplicate");
        return;
      }
      return;
    }
  }
  Node add;
  add.key = result;
  strcpy(add.value, string);
  dic[temp].push_back(add);
  return;
}

char search(unsigned token)
{
  int temp = hash(token);
  for (int i = 0; i < dic[temp].size(); i++) {
    if (dic[temp][i].key == token) {
      printf("%s\n", dic[temp][i].value);
      if (strlen(dic[temp][i].value) < 9)
        return dic[temp][i].value[0];
      else
        return '\0';
    }
  }
  printf("No\n");
  return '\0';
}

void init()
{
  char temp[10];
  for (int len = 1; len <= 5; len++) {
    int index[5] = { 0, 0, 0, 0, 0 };
    temp[len] = '\0';
    while (1) {
      for (int i = 0; i < len; i++) {
        temp[i] = alpha[index[i]];
      }
      unsigned raw = crc32(0, (unsigned char*)temp, len);
      unsigned result = crc32(raw, (unsigned char*)salt, strlen(salt));
      set(result, temp);

      index[0]++;
      int add = 0;
      while (index[add] >= (int)strlen(alpha)) {
        index[add] = 0;
        index[++add]++;
        if (add >= len)
          break;
      }
      if (add >= len)
        break;
    }
  }
}

int main()
{
  scanf("%d", &n);
  scanf("%s", salt);
  init();
  Vector<char> new_pw;
  for (int i = 0; i < n; i++) {
    unsigned token;
    scanf("%x", &token);
    char temp = search(token);
    if (temp != '\0') {
      char temp_pw[20];
      new_pw.push_back(temp);
      if (new_pw.size() > 8) {
        new_pw.pop_front();
      }
      for (int i = 0; i < new_pw.size(); i++) {
        temp_pw[i] = new_pw[i];
      }
      temp_pw[new_pw.size()] = '\0';
      for (int i = 0; i < new_pw.size(); i++) {
        unsigned raw = crc32(0, (unsigned char*)temp_pw + i, new_pw.size() - i);
        unsigned result = crc32(raw, (unsigned char*)salt, strlen(salt));
        set(result, temp_pw + i);
      }
    }
  }
}
