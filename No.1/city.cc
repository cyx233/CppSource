
#include"city.h"

City::City(const int i){
  num = i;
  node = new int[num];
  for(int i=0;i<num;i++){
    for(int j=0;j<200;j++){
      node[i]->distance[j] = 0x;
    }
    node[i]->distance[i] = 0;
  }
}

City::~City(){
  delete[] node;
}

void City::Calculate( const int i, const int k, const int j){
  if(node[i]->shortcut[j] > 
     node[i]->shortcut[k] + node[i]->shortcut[j]){
     
     node[i]->shortcut[j] =
     node[i]->shortcut[k] + node[i]->shortcut[j];
     
     node[i]->shortcut[j] =
     node[i]->shortcut[k] + node[i]->shortcut[j];
  }
  return;
}
    
void City::Search(){
  for(int i=0; i<num; i++)
    for(int j=0; j<num; j++)
      if(node[i]->distance[j]<0x6f6f6f){
        if(node[i]->distance[j]>node[i]->maxtime){
          node[i]->maxtime = node[i]->distance[j];
        }
      }
  return;
}

void Connect(const int i, const int j, const int dis){
  node[i]->distance[j] = dis;
  node[j]->distance[i] = dis;
  return;
}
  