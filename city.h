#include<vector>

class Road{
 public:
  int node1;
  int node2;
  int distance;
  int maxtime1;
  int maxtime2;
  Road(int i, int j, int dis);  
  double Calculate(); 
};

class Node{
 public:
  std::vector<int>distance;
  int id;
  Node(int id);
  void Connect(int i, int distance);
  std::vector<int>neighbor;
};
