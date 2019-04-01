#ifndef NODE_H
#define NODE_H
#include<vector>

class Node{
 private:
  int node_id;
  int x_position;
  int y_position;
  std::vector<int>neighbor;

 public:
  Node(int id, int x, int y);
  int x() const {return x_position;};
  int y() const {return y_position;};
  int id() const {return node_id;};
  void Connect(int id){neighbor.push_back(id);};
};

Node::Node(int id, int x, int y):node_id(id), x_position(x), y_position(y){}
#endif
