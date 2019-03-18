

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
  void SearchSec(const int firid, const int tar, const int secid);
};

struct Node{
  int neighbor[200]={0};
  int distance[200];
  int maxtime = 0;
  int sectime = 0;
  int maxid = -1;
};