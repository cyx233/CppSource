/* ====================================================
#   Copyright (C)2019 All rights reserved.
#
#   Author        : Yuxiang Chen
#   Email         : chenyuxi18@mails.tsinghua.edu.cn
#   File Name     : main.cpp
#   Last Modified : 2019-12-10 22:48
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
  unsigned key;   //密钥
  char value[10]; //字符串
};

const char alpha[25] = "0123456789tsinghua"; //所有的合法字符
char salt[10];                               //盐
const int BIG_P = 2501003;                   // hash函数所需质数
Vector<Node> dic[BIG_P + 10];                //字典
int n;

inline int hash(unsigned input)
{ //返回密钥的hash值
  ll temp = input;
  temp %= BIG_P;
  temp = (temp * 2018011253) % BIG_P; // hash化
  return (int)temp;
}

//密钥为result，字符串为string
inline void set(const unsigned result, const char* string)
{
  int temp = hash(result); //获取密钥hash值
  for (int i = 0; i < (int)dic[temp].size(); i++) {
    if (dic[temp][i].key == result) {            //若密钥已存在字典
      if (strcmp(dic[temp][i].value, string)) {  //且字符串不同
        strcpy(dic[temp][i].value, "Duplicate"); //设为重复
        return;
      }
      return;
    }
  }
  Node add;
  add.key = result;
  strcpy(add.value, string);
  dic[temp].push_back(add); //将密钥与字符串加入字典
  return;
}

//查询密钥token所对应字符串
inline char search(unsigned token)
{
  int temp = hash(token); //获取密钥hash值
  for (int i = 0; i < dic[temp].size(); i++) {
    if (dic[temp][i].key == token) {      //若密钥存在字典中
      printf("%s\n", dic[temp][i].value); //输出对应字符串
      if (strlen(dic[temp][i].value) < 9) //若字符串不为"Duplicate"
        return dic[temp][i].value[0];     //返回第一个字符
      else
        return '\0'; //若为"Duplicate"，返回'\0'
    }
  }
  printf("No\n"); //若密钥不存在字典中，输出No
  return '\0';    //返回'\0'
}

inline void init()
{ //初始化字典，加入长度小于等于5的密钥
  char temp[10];
  for (int len = 1; len <= 5; len++) { //遍历长度1~5
    int index[5] = { 0, 0, 0, 0, 0 };  //视为len位的18进制数
    temp[len] = '\0';
    while (1) {
      for (int i = 0; i < len; i++) {
        temp[i] = alpha[index[i]]; //获得对应字符串
      }
      unsigned raw = crc32(0, (unsigned char*)temp, len);
      unsigned result = crc32(raw, (unsigned char*)salt, strlen(salt)); //加密
      set(result, temp);                                                //保存密钥

      index[0]++; //最低位加1
      int add = 0;
      while (index[add] >= (int)strlen(alpha)) { //进位
        index[add] = 0;
        index[++add]++;
        if (add >= len) //已遍历len位的所有字符串
          break;
      }
      if (add >= len) //已遍历len位的所有字符串
        break;
    }
  }
}

int main()
{
  scanf("%d", &n);
  scanf("%s", salt);
  init();
  Vector<char> new_pw; //记录弱口令
  for (int i = 0; i < n; i++) {
    unsigned token;
    scanf("%x", &token);
    char temp = search(token); //搜索字符串并输出结果
    if (temp != '\0') {        //若成功破解
      char temp_pw[20];
      new_pw.push_back(temp);  //将字符串加入至弱口令末尾
      if (new_pw.size() > 8) { //弱口令长度不超过８
        new_pw.pop_front();
      }
      if (new_pw.size() <= 5) //长度不超过５的所有字符串不需要重复加入
        continue;
      for (int i = 0; i < new_pw.size(); i++) {
        temp_pw[i] = new_pw[i]; //获得当前存储的弱口令
      }
      temp_pw[new_pw.size()] = '\0';
      for (int i = 0; i < new_pw.size() - 5; i++) { //加入６~８位的弱口令
        unsigned raw = crc32(0, (unsigned char*)temp_pw + i, new_pw.size() - i);
        unsigned result = crc32(raw, (unsigned char*)salt, strlen(salt)); //加密
        set(result, temp_pw + i);                                         //保存密钥
      }
    }
  }
  return 0;
}
