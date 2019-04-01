#ifndef GRAGH_H
#define GRAGH_H
#include<vector>
#include<algorithm>
#include"node.h"
#include"edge.h"

bool edge_cmp(const Edge &a, const Edge &b){
  return(a.dis() < b.dis());
}
class Graph{
 private:
   std::vector<Node> nodes;
   std::vector<Edge> edges;
 public:
   void AddNode(Node n){nodes.push_back(n);};
   void AddEdge(Edge e){edges.push_back(e);};
   Node node(int i){return nodes[i];};
   Edge edge(int i){return edges[i];};
   void Sort(){sort(edges.begin(),edges.end(),edge_cmp);};
   int edge_num(){return edges.size();};
};
#endif
