#include <cstdlib>
#include <limits.h>
#include <cstdio>
#include "temperature.h"
using namespace std;
const int N = 600005;

inline int max(int a, int b){
  return a>b?a:b;
}

inline int min(int a, int b){
  return a<b?a:b;
}


//kdTree节点
struct Node {
  int d[2];//d[0]横坐标x，d[1]纵坐标y
  int tmp;//温度
  int mn[2];//mn[0]子树x左边界，mn[1]子树y左边界
  int mx[2];//mx[0]子树x右边界，mx[1]子树y右边界
  int mntmp, mxtmp;//mntmp子树最低气温，mxtmp子树最高气温
  int l, r;//l为左孩子下标，r为右孩子下标
} p[N];
int cur_cmp;//当前排序的维度，0为x，1为y

//比较函数cmp，降序
int cmp(const void *a,const void *b){
  if(((Node*)a)->d[cur_cmp] < ((Node*)b)->d[cur_cmp])
    return 1;
  if(((Node*)a)->d[cur_cmp] > ((Node*)b)->d[cur_cmp])
    return -1;
  return 0;
}

//若a属于b,return 1
bool inside(const Node &a, const Node &b) {
  return a.mn[0] >= b.mn[0] &&  a.mn[1] >= b.mn[1] && a.mx[0] <= b.mx[0] && a.mx[1] <= b.mx[1];
}

//两条平行线段，若垂线方向投影相交,return 1
bool linecross(int l1, int r1, int l2, int r2) {
  return max(l1, l2) <= min(r1, r2);
}

//若点在线段里，return 1
bool inrange(int x, int a, int b) {
  return x >= a && x <= b;
}

//两个矩形区域相交，等价于平行的两组边投影相交
//若区域相交，return 1
bool cross(const Node &a, const Node &b) {
  return linecross(a.mn[0], a.mx[0], b.mn[0], b.mx[0]) &&
    linecross(a.mn[1], a.mx[1], b.mn[1], b.mx[1]);
}

//kdTree
struct kdtree {
  Node t[N], now;//t为所有节点,now用于存当前查询区间
  int ans;//当前最值
  int root;//根节点
  int x1, x2, y1, y2;//查询区间

  //从孩子更新k的信息
  void update(int k) {
    int l = t[k].l, r = t[k].r;
    for(int i=0; i<=1; i++) {//更新2个维度的最值，即获得子树的区域
      t[k].mn[i] = t[k].mx[i] = t[k].d[i];
      if(l) {//从左孩子更新最值
        t[k].mx[i] = max(t[k].mx[i], t[l].mx[i]);
        t[k].mn[i] = min(t[k].mn[i], t[l].mn[i]);
      }
      if(r) {//从右孩子更新最值
        t[k].mx[i] = max(t[k].mx[i], t[r].mx[i]);
        t[k].mn[i] = min(t[k].mn[i], t[r].mn[i]);
      }
    }
    t[k].mntmp = t[k].mxtmp = t[k].tmp;//更新温度最值
    if(l) {//同上
      t[k].mntmp = min(t[k].mntmp, t[l].mntmp);
      t[k].mxtmp = max(t[k].mxtmp, t[l].mxtmp);
    }
    if(r) {//同上
      t[k].mntmp = min(t[k].mntmp, t[r].mntmp);
      t[k].mxtmp = max(t[k].mxtmp, t[r].mxtmp);
    }
  }

  //初始化树，节点为p[l]至p[r]共r-l+1个
  int build(int l, int r, int opt) {
    if(l > r)
      return 0;//特判叶子节点
    cur_cmp = opt;//目前排序的维度，0为x，1为y
    int mid = (l + r) >> 1;//二分,从中位数切分
    qsort(p+l, r-l+1, sizeof(Node), cmp);//快排
    t[mid] = p[mid];
    //每次递归更换维度
    t[mid].l = build(l, mid - 1, opt ^ 1);//递归左孩子
    t[mid].r = build(mid + 1, r, opt ^ 1);//递归右孩子
    update(mid);//更新最值
    return mid;//节点编号即为中位编号
  }

  //查询最大值
  void askmx(int k) {
    if(t[k].mxtmp <= ans)//剪枝
      return;
    if(inside(t[k], now)) {//判断子树区间是否被查询完全覆盖
      ans = max(ans, t[k].mxtmp);//若覆盖，更新当前最值信息
      return;
    }
    if(!cross(t[k], now))return;//子树区间与目标区间无交集

    //若节点两个坐标均属于查询区间对应的边范围内
    //说明子树根节点属于目标区间
    if(inrange(t[k].d[0], now.mn[0], now.mx[0]) &&
        inrange(t[k].d[1], now.mn[1], now.mx[1])) {
      ans = max(ans, t[k].tmp);//更新当前最值信息
    }
    int mxl = -1, mxr = -1, l = t[k].l, r = t[k].r;//获得左右孩子的最大温度
    if(l)
      mxl = t[l].mxtmp;
    if(r)
      mxr = t[r].mxtmp;
    if(mxl > mxr) {//优先搜索可能有大值的区间，利于剪枝
      if(l)askmx(l);
      if(r)askmx(r);
    }
    else {
      if(r)askmx(r);
      if(l)askmx(l);
    }
  }

  //查询最大值, 与查询最大值同理
  void askmn(int k) {
    if(t[k].mntmp >= ans)
      return;
    if(inside(t[k], now)) {
      ans = min(ans, t[k].mntmp);
      return;
    }
    if(!cross(t[k], now))
      return;
    if(inrange(t[k].d[0], now.mn[0], now.mx[0]) &&
        inrange(t[k].d[1], now.mn[1], now.mx[1])) {
      ans = min(ans, t[k].tmp);
    }
    int mnl = INT_MAX, mnr = INT_MAX, l = t[k].l, r = t[k].r;
    if(l)
      mnl = t[l].mntmp;
    if(r)
      mnr = t[r].mntmp;
    if(mnl < mnr) {
      if(l)askmn(l);
      if(r)askmn(r);
    }
    else {
      if(r)askmn(r);
      if(l)askmn(l);
    }
  }

  //查询x1,x2,y1,y2区间，mode为0查询最小值，mode为1查询最大值
  int ask(int x1, int x2, int y1, int y2, int mode) {
    //now仅仅用于记录查询区间
    now.mn[0] = x1;
    now.mx[0] = x2;
    now.mn[1] = y1;
    now.mx[1] = y2;
    if(mode == 0) {
      ans = INT_MAX;
      askmn(root);
      return ans;
      //见函数query，若查询最小值，那么一定有观测站，直接输出结果
    }
    else {
      ans = -1;//若没有观测站，查询结果应为-1
      askmx(root);
      return ans;
    }
  }
} t;

//初始化树
void init(int n, const int *x, const int *y, const int *temp) {
  for(int i=1; i<=n; i++) {
    p[i].d[0] = x[i - 1];//d[0]存x
    p[i].d[1] = y[i - 1];//d[1]存y
    p[i].tmp = temp[i - 1];
  }
  t.root=t.build(1, n, 0);//建树
}

void query(int x1, int x2, int y1, int y2, int *tmin, int *tmax) {
  *tmax = t.ask(x1, x2, y1, y2, 1);
  if(*tmax == -1)//最大值一定非负，若查询结果为-1，说明没有观测站
    *tmin = -1;//没有观测站输出-1，-1
  else//不能用min判断，因为min最大值为INT_MAX，结果为INT_MAX不能判断有没有观测站
    *tmin = t.ask(x1, x2, y1, y2, 0);
}
