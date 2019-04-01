#ifndef EDGE_H
#define EDGE_H
#include<vector>
#include<cmath>
#include"node.h"

//边
class Edge{
 private:
  int edge_id;
  Node node1;//端点
  Node node2;
  float distance;//长度
 public:
  //调用查看
  Edge(int id, Node n1, Node n2);
  Node n1() const {return node1;};
  Node n2() const {return node2;};
  int id(){return edge_id;}
  float dis() const {return distance;};
};

Edge::Edge(int id, Node n1, Node n2):edge_id(id), node1(n1), node2(n2){
  int dx = abs(node1.x() - node2.x()); 
  int dy = abs(node1.y() - node2.y()); 
  distance = sqrt(dx*dx + dy*dy);//计算长度
}
#endif
