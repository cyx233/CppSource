#ifndef GRAGH_H
#define GRAGH_H #include<vector>
#include<algorithm>
#include<vector>
#include"node.h"
#include"edge.h"

//排序辅助函数
bool edge_cmp(const Edge &a, const Edge &b){
  return(a.dis() < b.dis());
}

//图
class Graph{
 private:
   std::vector<Node> nodes;
   std::vector<Edge> edges;
   void Parent(int i, int j){nodes[i].parent = j;}//并查集-祖先结点修改
   int Find(int x);//并查集-查找
   void Union(int x, int y);//并查集-合并

 public:
   //添加边与结点
   void AddNode(Node n){nodes.push_back(n);};
   void AddEdge(Edge e){edges.push_back(e);};

  //调用查看
   Node node(int i){return nodes[i];};
   Edge edge(int i){return edges[i];};
   int edge_num(){return edges.size();};
   int node_num(){return nodes.size();};

  //对图中的边以长度排序
   void Sort(){sort(edges.begin(),edges.end(),edge_cmp);};

  //判断回路
   bool IsCycle(Edge curedge);
};
#endif
