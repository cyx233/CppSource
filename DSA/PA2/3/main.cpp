#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
# define DEFAULT_CAPACITY 3
const int M=400040, N=100010;


inline int min(int a, int b){
  return a<b?a:b;
}
inline int max(int a, int b){
  return a>b?a:b;
}

//课件上的Vector模板类
template <typename T> class Vector{
  private:
    int _size;
    int _capacity;
    T* _elem;
  protected:
    void copyFrom(T const* A, int lo, int hi){//拷贝
      _elem = new T[_capacity = 2*(hi-lo)];
      _size = 0;
      while(lo<hi)
        _elem[_size++] = A[lo++];
    }
    void expand(){//扩容
      if(_size<_capacity)return;
      _capacity = max(_capacity, DEFAULT_CAPACITY);
      _capacity = _capacity<<1;
      T* oldElem = _elem; _elem = new T[_capacity];
      for(int i=0; i<_size; i++)
        _elem[i] = oldElem[i];
      delete[] oldElem;
    }

    void shrink(){//缩减
      if(_size + 1 > _capacity/2)
        return;
      _capacity = max(_capacity, DEFAULT_CAPACITY<<1);
      _capacity = _capacity>>1;
      T* oldElem = _elem; _elem = new T[_capacity];
      for(int i=0; i<_size; i++)
        _elem[i] = oldElem[i];
      delete[] oldElem;
    }

    //比较函数cmp
    static int compare_ints(const void *p, const void *q) {
      int x = *(const int *)p;
      int y = *(const int *)q;

      if (x < y)
        return -1; //升序
      else if (x > y)
        return 1;

      return 0;
    }


  public:
    //默认构造
    Vector(int c = DEFAULT_CAPACITY){
      _elem = new T[_capacity = c];
      _size = 0;
    }
    //数组拷贝构造
    Vector(T const& A, int lo, int hi){
      copyFrom(A, lo, hi);
    }
    //部分拷贝构造
    Vector(Vector<T> const& V, int lo, int hi){
      copyFrom(V._elem, lo, hi);
    }
    //拷贝构造
    Vector(Vector<T> const& V){
      copyFrom(V._elem, 0, V._size);
    }
    ~Vector(){
      delete [] _elem;
    }

    //将元素e插入到第r个
    int insert(int r, T const & e){
      expand();
      for(int i=_size; i>r; i--){
        _elem[i] = _elem[i-1];
      }
      _elem[r]=e;
      _size++;
      return r;
    }
    //移除[lo, hi)
    int remove(int lo, int hi){
      if(lo == hi) return 0;
      while(hi < _size)
        _elem[lo++] = _elem[hi++];
      _size = lo;
      shrink();
      return hi - lo;
    }
    //移除第r个
    T remove(int r){
      T e = _elem[r];
      remove(r, r+1);
      return e;
    }
    //在末尾插入
    int push_back(T const& e){
      return insert(_size, e);
    }
    //重载[]
    T& operator[](int r){
      return _elem[r];
    }
    //返回size
    int size(){
      return _size;
    }
    //排序（升序）
    void sort(){
      qsort(_elem, _size, sizeof *_elem, compare_ints);
    }
};

int dfn[N];     //时间戳
int dfs_clock;  //时间计数器
bool is_cut[N]; //节点是否为割点
int dcc_cnt;    //双连通分量计数器
int new_id[N];  //缩点后原先节点新的下标
int old_id[N];  //缩点后的割点对应缩点前的下标
int stack[N];   //栈用于生成双联通分量
int top;        //栈顶
Vector<int> dcc[N];         //双联通分量，记录包含点
Vector<int> nodes[N];       //原图节点，邻接表
Vector<int> new_node[2*N];  //双联通分量+割点，缩点成新图最多节点2N-2个

//加入无向边u-v
void adde(int u, int v){
  nodes[u].push_back(v);
  nodes[v].push_back(u);
}


//tarjan算法
int dfs(int u, int parent){
  int lowu;
  dfn[u] = lowu = ++dfs_clock;//打上时间戳
  stack[++top] = u;//节点u入栈
  if(!parent && !nodes[u].size())//若图有且仅有一个点
  {
    dcc[++dcc_cnt].push_back(u);
    return 0;
  }
  int flag = 0;//判断根节点是否为割点比较特殊
  for(int i=0; i<nodes[u].size(); i++){
    int v = nodes[u][i];//孩子v
    if(!dfn[v]){//若v未被访问
      int lowv = dfs(v, u);//递归v，函数返回值为lowv，具体定义见解题报告
      lowu = min(lowu, lowv);
      if(lowv >= dfn[u]){//判断u是否为割点，具体算法解释见解题报告
        flag++;
        if(parent || flag>1)//根节点特判，其余节点parent!=0
          is_cut[u]=true;
        dcc_cnt++;
        int w;
        while(1){//从栈中取出双联通分量包含的点
          w=stack[top--];
          dcc[dcc_cnt].push_back(w);
          if(w == v)//v被u所割，当出栈至v时，双联通分量完全出栈
            break;
        }
        dcc[dcc_cnt].push_back(u);//双联通分量包含割点
      }
    }
    else lowu = min(lowu, dfn[v]);//更新lowu
  }
  return lowu;//递归所需
}

//查询a-b之间的必经点（不包含a，b），ans为答案
void search(int a, int b, Vector<int>& ans){
  int u = new_id[a];//寻找a在新图中的对应点
  int v = new_id[b];//寻找b在新图中的对应点
  if(u==v)//若在新图上对应同个点，说明没有必经点
    return;
  int sch_stk[2*N];//深搜所用栈
  int path[2*N];//记录深搜路径
  bool visit[2*N];//深搜记录是否访问过
  memset(visit, 0 ,sizeof(visit));//初始化
  int top = 1;
  sch_stk[0] = u;//顶点入栈
  while(top>0){
    int c = sch_stk[--top];//栈顶弹出，为当前节点
    if(visit[c]){//若访问过，跳过
      continue;
    }
    if(c == v){//若当前为终点，进行路径回溯
      int cur=path[v];
      while(cur!=u){//回溯路径
        //判断是否为割点，
        //在存储时，特意使割点在新图对应下标大于双联通分量总数dcc_cnt
        if(cur>dcc_cnt){
          ans.push_back(old_id[cur-dcc_cnt]);//若为割点，说明是必经点之一
        }
        cur = path[cur];//回溯
      }
      return;
    }
    visit[c] = 1;//标记c被访问
    for(int i=0; i<new_node[c].size(); i++){//遍历c的邻点
      if(!visit[new_node[c][i]]){//若未被访问
        sch_stk[top++] = new_node[c][i];//进栈
        path[new_node[c][i]] = c;//记录路径
      }
    }
  }
  return;
}

int main(){
  int n, m;
  //读入数据
  scanf("%d%d", &n, &m);
  for(int i=0; i<m; i++){
    int u, v;
    scanf("%d%d", &u, &v);
    adde(u, v);//初始化原图
    adde(v, u);
  }
  dfs(1, 0);//标记割点，存储双联通分量

  int new_cnt = dcc_cnt;//割点在新图中对应点下标从dcc_cnt+1开始记录，便于判断
  for(int i=1; i<=dcc_cnt; i++)//遍历所有双联通分量
  {
    for(int j=0; j<dcc[i].size(); ++j){//遍历双联通分量中所有点
      int id = dcc[i][j];
      if(is_cut[id]){//若为割点
        if(!new_id[id]){//若此割点尚未分配对应点
          new_cnt++;
          new_id[id] = new_cnt;//分配对应点
          old_id[new_cnt - dcc_cnt] = id;//记录原下标
        }
        new_node[new_id[id]].push_back(i);//割点对应点与双联通分量对应点互相连接
        new_node[i].push_back(new_id[id]);
      }
      else{
        new_id[id] = i;//若不是割点，缩点进对应的双联通分量
      }
    }
  }
  int q;
  //读取查询数据
  scanf("%d", &q);
  for(int i=0; i<q; i++){
    int u, v;
    scanf("%d%d", &u, &v);
    Vector<int> ans;
    ans.push_back(u);//起点为必经点
    if(u!=v)//判断起点终点是否重合
      ans.push_back(v);//终点为必经点
    search(u, v, ans);//查询，结果记录于ans
    ans.sort();//升序输出
    for(int j=0; j<ans.size(); j++){
      printf("%d ", ans[j]);
    }
    printf("\n");
  }
  return 0;
}
