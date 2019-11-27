#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <assert.h>
# define DEFAULT_CAPACITY 3//Vector初始大小
const int  N=1000010;

int ZERO = 0;//代替NULL

inline int min(int a, int b){
  return a<b?a:b;
}
inline int max(int a, int b){
  return a>b?a:b;
}

template <typename T> class Vector{//模板类来自课件
  private:
    int _size;
    int _capacity;
    T* _elem;
  protected:
    void copyFrom(T const* A, int lo, int hi){//向量复制
      _elem = new T[_capacity = 2*(hi-lo)];
      _size = 0;
      while(lo<hi)
        _elem[_size++] = A[lo++];
    }
    void expand(){//向量扩容
      if(_size < _capacity)return;
      _capacity = max(_capacity, DEFAULT_CAPACITY);
      _capacity = _capacity<<1;
      T* oldElem = _elem; _elem = new T[_capacity];
      for(int i=0; i<_size; i++)
        _elem[i] = oldElem[i];
      delete[] oldElem;
    }

    void shrink(){//向量缩减
      if(_size  > _capacity>>2)
        return;
      _capacity = max(_capacity, DEFAULT_CAPACITY<<1);
      _capacity = _capacity>>1;
      T* oldElem = _elem; _elem = new T[_capacity];
      for(int i=0; i<_size; i++)
        _elem[i] = oldElem[i];
      delete[] oldElem;
    }

  public:
    //默认构造
    Vector(int c = DEFAULT_CAPACITY){
      _elem = new T[_capacity = c];
      _size = 0;
    }
    //数组构造
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

    //在秩为r处插入元素e
    int insert(int r, T const & e){
      expand();//先扩容
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

    //移除秩为r
    T remove(int r){
      T e = _elem[r];
      remove(r, r+1);
      return e;
    }

    //在末尾插入
    int push_back(T const& e){
      return insert(_size, e);
    }

    //移除末尾元素
    int pop_back(){
      return remove(_size-1, _size);
    }

    //[]重载
    T& operator[](int r){
      if(r<0 || r>=_size)
        return ZERO;//用0表示NULL, 避免出现RE
      return _elem[r];
    }

    //返回元素个数
    int size(){
      return _size;
    }

    //获取最后一个元素的值
    T last(){
      return _elem[_size-1];
    }
};

//树中节点
struct Node{
  int height=0;//节点高度
  int size=1;//节点规模
  int parent;//父节点id
  Vector<int>child;//子节点id
  Vector<int>aux;//Queap辅助数组
}nodes[N];//最多N个节点

//寻找nodes[c]是nodes[p]的第几个子节点(从0开始编号)
//若c不是子节点，返回-1
int find_rank(int p, int c){
  if(!p)
    return -1;
  int rank = -1;
  for(int i=nodes[p].child.size()-1; i>=0; i--){//向量中倒序存储，所以倒序查找
    if(nodes[p].child[i] == c){
      rank = nodes[p].child.size() - i - 1;//此处将倒序改为正序
      break;
    }
  }
  return rank;
}

//nodes[p]的第r个孩子（从0计数）获得更新，由此对p进行更新
int update_height_above(int p, int r){
  if(!p)//若p不存在
    return 0;
  int size = nodes[p].child.size();
  int rank = size-r-1;//向量中为倒序存储
  for(int i=size-1; i>=rank; i--){//Queap移除[0,r]元素
    nodes[p].aux.pop_back();
  }
  int max_h = nodes[p].aux.last();
  for(int i=rank; i<=size-1; i++){//Queap重新插入[0,r]元素
    max_h = max(max_h, nodes[nodes[p].child[i]].height);
    nodes[p].aux.push_back(max_h);
  }
  if(max_h+1 == nodes[p].height)//如果此改变不影响节点高度，则不在向上递归
    return 0;
  nodes[p].height = max_h+1;
  return 1;//否则向上递归
}



//将nodes[c]插入到nodes[p]作为第r个孩子（从0计数）
void insertAs(int p, int c, int r){
  if(!p)//若p不存在
    return;
  int size = nodes[p].child.size();
  int rank = size-r-1;//向量内倒序存储

  for(int i=size-1; i>rank; i--){//Queap移除[0,r]
    nodes[p].aux.pop_back();
  }

  //由于倒序, insert 插入在元素前，需要插入在rank+1
  nodes[p].child.insert(rank+1, c);
  nodes[c].parent = p;
  size++;

  int max_h = nodes[p].aux.last();

  for(int i=rank+1; i<=size-1; i++){//Queap插入[0,r+1]
    max_h = max(max_h, nodes[nodes[p].child[i]].height);
    nodes[p].aux.push_back(max_h);
  }

  if(max_h+1 != nodes[p].height){//若高度需要更新
    nodes[p].height = max_h+1;
    int cur=p;
    //递归更新父亲，直到根节点或者某节点高度不被此次更新影响
    while(update_height_above(nodes[cur].parent, find_rank(nodes[cur].parent, cur))){
      cur = nodes[cur].parent;
    }
  }

  int current = p;
  int size_c = nodes[c].size;
  //递归更新规模直至根节点
  while(current){
    nodes[current].size += size_c;
    current = nodes[current].parent;
  }
}

//移除nodes[p]的第r个子节点（从0计数）
int remove(int p, int r){
  if(!p)//若p不存在
    return 0;
  int size = nodes[p].child.size();
  int rank = size-r-1;//向量中倒序存储

  for(int i=size-1; i>=rank; i--){//Queap移除[0,r]
    nodes[p].aux.pop_back();
  }

  int c = nodes[p].child.remove(rank);
  nodes[c].parent = 0;
  size--;
  if(!nodes[p].child.size()){//叶子节点高度特判为0
    nodes[p].height = 0;
    int cur=p;
    //递归更新父亲高度，直到根节点或者某节点高度不被此次更新影响
    while(update_height_above(nodes[cur].parent, find_rank(nodes[cur].parent, cur))){
      cur = nodes[cur].parent;
    }
  }
  else{
    int max_h = nodes[p].aux.last();
    for(int i=rank; i<=size-1; i++){//Queap插入[0,r-1]
      max_h = max(max_h, nodes[nodes[p].child[i]].height);
      nodes[p].aux.push_back(max_h);
    }
    if(max_h+1 != nodes[p].height){//若高度被更新
      nodes[p].height = max_h+1;
      int cur=p;
      //递归更新父亲高度，直到根节点或者某节点高度不被此次更新影响
      while(update_height_above(nodes[cur].parent, find_rank(nodes[cur].parent, cur))){
        cur = nodes[cur].parent;
      }
    }
  }
  int current = p;
  int size_c = nodes[c].size;

  //递归更新父亲规模，直到根节点
  while(current){
    nodes[current].size -= size_c;
    current = nodes[current].parent;
  }
  return c;
}


int dfn[2*N];//层次遍历的栈
int list[2*N];//模拟队列

void init(){
  int cnt=0;
  int head=1;
  int tail=0;
  list[0] = 1;
  while(head>tail){//广搜（层次遍历）
    int cur = list[tail++];
    dfn[cnt++] = cur;//当前节点入栈
    for(int i=0; i<nodes[cur].child.size(); i++){
      list[head++] = nodes[cur].child[i];
    }
  }

  for(int i=cnt-1; i>=0; i--){//出栈并根据子节点更新高度和规模
    int id = dfn[i];
    nodes[id].size = 1;
    for(int j=0; j<nodes[id].child.size(); j++){
      int c = nodes[id].child[j];

      nodes[id].size += nodes[c].size;

      nodes[id].height = max(nodes[id].height, nodes[c].height+1);
      nodes[id].aux.push_back(nodes[id].height-1);
    }
  }
}

int path2node(int argc[]){//由题意根据路径寻找节点
  int len = argc[0];//第一位是路径长度
  int current = 1;

  for(int i=1;i<=len;i++){
    int rank = nodes[current].child.size() - argc[i] - 1;
    if(rank<0)//若路径不合法
      return current;//返回最后一个合法节点
    current = nodes[current].child[rank];
  }
  return current;
}


int main(){
  int m,n;
  int argc[N];
  //读入数据
  scanf("%d%d", &n, &m);
  for(int i=1; i<n+1; i++){
    scanf("%d",&argc[0]);
    for(int j=1;j<=argc[0];j++){
      scanf("%d", &argc[j]);
    }
    for(int j=argc[0]; j>=1; j--){//向量中倒序存储
      nodes[i].child.push_back(argc[j]);
      nodes[argc[j]].parent = i;
    }
  }

  init();//初始化高度和规模


  for(int i=0;i<m;i++){//进行m个操作
    int type; scanf("%d",&type);
    switch(type){//区分操作
      case 0:{//子树移动
               int len;
               //读入数据
               scanf("%d", &len);
               argc[0] = len;
               for(int i=1; i<=len; i++){
                 scanf("%d", &argc[i]);
               }
               int node1 = path2node(argc);//源子树的根节点

               //移除源子树
               remove(nodes[node1].parent, find_rank(nodes[node1].parent,node1));

               scanf("%d", &len);
               argc[0] = len;
               for(int i=1; i<=len; i++){
                 scanf("%d", &argc[i]);
               }

               int node2 = path2node(argc);//目标父节点
               int pos2;
               scanf("%d", &pos2);

               //插入子树
               insertAs(node2, node1, pos2);
               break;
             }
      case 1:{//查询高度
               int len;
               //读入数据
               scanf("%d", &len);
               argc[0] = len;
               for(int i=1; i<=len; i++){
                 scanf("%d", &argc[i]);
               }
               int node1 = path2node(argc);//目标节点
               printf("%d\n", nodes[node1].height);
               break;
             }
      case 2:{//查询规模
               int len;
               //读入数据
               scanf("%d", &len);
               argc[0] = len;
               for(int i=1; i<=len; i++){
                 scanf("%d", &argc[i]);
               }
               int node1 = path2node(argc);//目标节点
               printf("%d\n", nodes[node1].size);
               break;
             }
    }
  }
  return 0;
}
