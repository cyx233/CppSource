#ifndef NODE_H
#define NODE_H
#include<vector>

//结点
class Node{
 private:
  int node_id;
  int x_position;
  int y_position;

 public:
  int parent;//并查集-祖先结点
  Node(int id, int x, int y);
  int x() const {return x_position;};
  int y() const {return y_position;};
  int id() const {return node_id;};
};

Node::Node(int id,int x,int y):node_id(id), x_position(x),
                               y_position(y){parent = id;}//祖先结点初始化为自身
#endif
