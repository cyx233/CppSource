
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
  for(int i=0; i<num; i++){
    for(int j=0; j<num; j++){
      if(node[i]->distance[j]<0x6f6f6f){
        if(node[i]->distance[j]>node[i]->maxtime){
          SearchSec(j,i,node[i]->maxid);
          node[i]->maxtime = node[i]->distance[j];
        }
      }
    }
    
    if(node[i]->sectime == 0){
      for(int j=0; j<num; j++){
        if(node[i]->distance[j]>node[i]->sectime&&
           node[i]->distance[j]<node[i]->maxtime){
          SearchSec(j,i,node[i]->maxid);
        }
      }
    } 
  }
  return;
}

void Connect(const int i, const int j, const int dis){
  node[i]->distance[j] = dis;
  node[j]->distance[i] = dis;
  node[i]->neighbor[j] = 1;
  node[j]->neighbor[i] = 1;
  return;
}

void City::SearchSec(const int firid, const int tar, const int secid){
  if(secid == -1){
    node[tar]->maxid = firid;
    return;
  }
  
  int fircom,seccom,firmin=0xffffff,secmin=0xffffff;
  for(int i=1;i<n;i++){
    if(node[tar]->neighbor[i] == 1){
      if(node[firid]->distance[i]+node[tar]->distance[i]<firmin){
        firmin = node[firid]->distance[i]+node[tar]->distance[i];
        fircom = i;
      }
      if(node[secid]->distance[i]<secmin){
        secmin = node[secid]->distance[i];
        seccom = i;
      }
    }  
  }
  
  if(fircom == seccom)
    return;
  else{
    node[tar]->sectime = node[secid]->distance[tar];
  }
  return;
}
  