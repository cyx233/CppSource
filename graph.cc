#include"node.h"
#include"edge.h"
#include"graph.h"

//并查集-查找
int Graph::Find(int x){
  int r = x;
  while(node(r).parent != r)
    r = node(r).parent;
  int i = x, j; 
  while(i != r){
    j = node(i).parent;
    Parent(i, r); 
    i = j;
  }
  return i;
}

//并查集-合并
void Graph::Union(int x, int y){
  int fx = Find(x);
  int fy = Find(y);
  if(fx == fy)
    return;
  if(fx < fy)
    Parent(x,y);
  else{
    Parent(y,x); 
  }
} 

//判断回路
bool Graph::IsCycle(Edge curedge){
  int x = curedge.n1().id();
  int y = curedge.n2().id();

  int fx = Find(x);
  int fy = Find(y);
  if(fx == fy)//若新增一边的两个端点有相同的祖先结点,说明形成回路
    return true;
  else{
    Union(x, y);//若不形成回路,则合并两端点所在的集合
    return false;
  }
}
