

class City{
 private:
  int num;
  struct Node;
  Node* node;
 public:
  City(int num);
  ~City();
  void Calculate(const int i, const int k, const int j);
  void Search();
  void Connect(const int i, const int j, const int dis)
};

struct Node{
  int distance[200];
  int maxtime = 0;
};