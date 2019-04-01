#ifndef EDGE_H
#define EDGE_H
#include<vector>
#include<cmath>
#include"node.h"

class Edge{
 private:
  int id;
  Node node1;
  Node node2;
  float distance;
 public:
  Edge(int id, Node n1, Node n2);
  Node n1() const {return node1;};
  Node n2() const {return node2;};
  float dis() const {return distance;};
};

Edge::Edge(int id, Node n1, Node n2):id(id), node1(n1), node2(n2){
  int dx = abs(node1.x() - node2.x()); 
  int dy = abs(node1.y() - node2.y()); 
  distance = sqrt(dx*dx + dy*dy);
}
#endif
