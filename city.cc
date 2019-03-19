#include<cmath>
#include"city.h"

Road::Road(int i, int j, int dis){
  node1 = i;
  node2 = j;
  distance = dis;
  maxtime1 = 0;
  maxtime2 = 0;
}

double Road::Calculate(){
  if(abs(this->maxtime1 - this->maxtime2) > this->distance){
    if(this->maxtime2 > this->maxtime1)
      return static_cast<double>(this->maxtime2);
    else
      return static_cast<double>(this->maxtime1);
  }
  else
    return static_cast<double>((this->maxtime1 + this->maxtime2 +
                                this->distance))/2.0;
}

void Node::Connect(int i, int dis){
  this->neighbor.push_back(i);
  this->distance.push_back(dis);
}

Node::Node(int id){
  this->id=id;
}
