#include <cstdio>
typedef long long int ll;

const int N=10500001;
int count=2;//节点计数器，根节点为1

//线段树节点
struct Node{
  int lc=0;//左孩子下标
  int rc=0;//右孩子下标
  int lazy_tag=0;//懒惰标记
  ll sum=0;//区间和
};

Node nodes[N];//所有节点理论上不超过mlogn

inline void push_down(int current, int l, int r){
  if(!nodes[current].lazy_tag)//若无懒惰标记
    return;

  int lc = nodes[current].lc;
  int rc = nodes[current].rc;
  int lazy = nodes[current].lazy_tag;

  if(!lc){//若没有左孩子
    nodes[current].lc = count;//分配左孩子
    lc = count;
    count++;
  }

  if(!rc){//右孩子同理
    nodes[current].rc = count;
    rc = count;
    count++;
  }

  int mid = ((ll)l+(ll)r)/2;//二分父节点区间
  nodes[lc].sum += (ll)lazy*(ll)(mid-l+1);//左孩子更新区间和
  nodes[lc].lazy_tag += lazy;//左孩子更新懒惰标记
  nodes[rc].sum += (ll)lazy*(ll)(r-mid);//右孩子同理
  nodes[rc].lazy_tag += lazy;

  nodes[current].lazy_tag = 0;//父节点懒惰标记清零
}


//当前节点current，区间范围[l,r], 修改区间[targetl, targetr]
inline void add(int& current, int l, int r, int targetl, int targetr){
  if(!current){//若当前节点不存在，分配节点
    current = count;
    count++;
  }

  //若查询区间完全覆盖当前区间，直接修改区间总和、懒惰标记
  if(l>=targetl && r<=targetr){
    nodes[current].sum += (ll)(r-l+1);
    nodes[current].lazy_tag++;
    return;
  }

  push_down(current, l, r);//懒惰标记下放

  int mid = ((ll)l+(ll)r)/2;//二分当前区间
  if(mid>=targetl)//若查询区间与左孩子有重叠，递归修改左孩子
    add(nodes[current].lc, l, mid, targetl, targetr);
  if(mid<targetr)//右孩子同理
    add(nodes[current].rc, mid+1, r, targetl, targetr);

  //由左右孩子更新区间总和
  nodes[current].sum =
    nodes[nodes[current].lc].sum + nodes[nodes[current].rc].sum;
}

//当前节点current，区间范围[l,r]，查询区间[targetl, targetr]
inline ll ask(int current, int l, int r, int targetl, int targetr){
  if(l>=targetl && r<=targetr){//若查询完全覆盖当前区间，直接返回区间总和
    return nodes[current].sum;
  }
  if(l>targetr || r<targetl)//若查询与当前区间不相交，返回0
    return 0;

  ll sum = 0;
  push_down(current, l, r);//懒惰标记下放

  int mid = ((ll)l+(ll)r)/2;//二分当前区间
  if(mid>=targetl){//若左孩子与查询有重叠，递归查询左孩子
    sum += ask(nodes[current].lc, l, mid, targetl, targetr);
  }
  if(mid<targetr){//右孩子同理
    sum += ask(nodes[current].rc, mid+1, r, targetl, targetr);
  }
  return sum;
}

int main(){
  int n;
  int m;
  int root = 1;
  //读取数据
  scanf("%d%d", &n, &m);
  for(int i=0; i<m; i++){
    char c[5];
    scanf("%s",c);
    //若为翻转操作，修改线段树
    if(c[0]=='H'){
      int tl, tr;
      scanf("%d%d", &tl, &tr);
      add(root,1,n,tl,tr);
    }
    //若为查询操作，查询线段树
    if(c[0]=='Q'){
      int tl, tr;
      scanf("%d%d", &tl, &tr);
      printf("%lld\n",ask(root,1,n,tl,tr));
    }
  }
  return 0;
}
