#include<cstdio>
using namespace std;
typedef unsigned long long ll;
const int N = 500005, K = 64, B = 4, D = (K / B) + 1;//四位做一个节点
const int MASK = (1 << B) - 1, STA=K%B?K-K%B:K-B;//由于64|4，需要特殊处理

int n, k, e_siz;//n，k为题中n，k
char s[66];//每个元件以字符串形式读入
int ans[N];//答案
ll a[N];//每个原件以long long形式存储

//邻接表存边(父节点-子节点)
struct edge {
  int nxt, t, id;//nxt是下一条边，t为子节点的值，id为子节点索引
  edge() {}
  edge(int nxt, int t, int id):
    nxt(nxt), t(t), id(id){}
} e[N * D];

//head为每个节点的子节点链表头
//siz用于判断每个节点代表的子树有几个叶子节点（删除信息需要本数组）
//cid用于存储叶子节点对应的元件id
int head[N * D], siz[N * D], cid[N * D];

//值为y，下标为id的点作为子节点连接到x
inline void adde(int x, int y, int id) {
  e_siz++;
  e[e_siz] = edge(head[x], y, id);
  head[x] = e_siz;
}

//在x的孩子中寻找在节点数组中对应下标为id的孩子
inline int find(int x, int id) {
  for(int k = head[x]; k; k = e[k].nxt)
    if(id == e[k].id)
      return e[k].t;
  return 0;
}

//trie_siz为总叶子数目，root为根节点下标
int trie_siz = 1, root = 1;

//x为当前节点下标，递归插入值为val的一条信息，代表元件序号为id
void insert(int x, ll val, int id, int d = STA) {
  siz[x]++;
  if(d < 0) {//若val已经完全被插入
    cid[x] = id;//下标为x叶子节点代表的元件为第id个
    return;
  }
  int p = (val >> d)&MASK;//每次取MASK对应的位数（4位）
  int y = find(x, p);//寻找是否已经存在值为p的子节点
  if(!y) {//若没有，出现新的分支
    y = ++trie_siz;
    adde(x, y, p);
  }
  insert(y, val, id, d - B);//递归插入val的更低位
}

//x为当前节点，递归从树中删除val
void del(int x, ll val, int d = STA) {
  siz[x]--;//必定会删除,
  if(d < 0)return;//d控制递归次数, 共16次
  int p = (val >> d)&MASK;//每次取出MASK对应的位（4位）
  int y = find(x, p);//由于删除的信息必定存在树中，y!=0
  del(y, val, d - B);//递归删除
}
//x为当前节点，递归查询与val异或最大值对应的叶子
int ask(int x, ll val, int d = STA) {
  if(d < 0)//d控制递归层数
    return cid[x];//返回叶子编号
  int p = (val >> d)&MASK;
  int y = 0, mx = 0,flag=0;
  for(int k = head[x]; k; k = e[k].nxt)
    //通过判断的子树有几个叶子判断x是否有效
    //flag是首次遇到存在叶子的孩子，特判更新mx
    //其余情况高位贪心
    if(siz[e[k].t] && (!flag || (e[k].id ^ p) > mx)) {
      flag=1;
      mx = e[k].id ^ p;
      y = e[k].t;
    }
  return ask(y,val,d-B);//递归查询val低位
}

inline int max(int a, int b){
  return a>b?a:b;
}

inline int min(int a, int b){
  return a<b?a:b;
}

int main() {
  scanf("%d%d", &n, &k);
  for(int i=0; i<n; i++){
    scanf("%s", s);
    ll x = 0;
    for(int i=0; i<K; i++)//由于题意，倒序读入，输出符合条件的所有元件的最小序号
      x |= ((ll)s[63 - i] - '0') << i;//将字符串转化为64位整型
    a[n-i-1] = x;
  }

  int lx = -1, rx = -1;
  for(int i=0; i<n; i++){//计算第i个元件对应的答案
    int nlx = max(-1, i - k - 2);//(nlx, nrx]区间
    int nrx = min(n - 1, i + k + 1);
    while(nlx > lx) {
      lx++;
      del(root, a[lx]);//删除(lx,nlx]
    }
    while(nrx > rx) {
      rx++;
      insert(root, a[rx], rx);//插入(rx,nrx]
    }
    ans[i]=ask(root,a[i]);//记录答案
  }
  int temp = n-ans[n-1]-1;

  if(temp){//特判第0个至第k个均相等的情况，应该输出1而不是输出0
    printf("%d\n", temp);
  }
  else{
    printf("1\n");
  }

  for(int i=1; i<n; i++)//其余的不可能与自己异或最大，正常输出
    printf("%d\n",n-ans[n-i-1]-1);//倒序输出，由于倒序读入，所以结果为正序
}
