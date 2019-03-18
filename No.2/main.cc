#include<cstdio>
#include"city.h"

int main(){
  int n=0, m=0;
  std::scanf("%d%d",&n,&m);
  
  City thu(n);
  
  for(int i=0; i<m; i++){
    int node1, node2, dis;
    scanf("%d%d%d",&node1,&node2,&dis);
    thu.Connect(node1,node2,dis);
  }
  
  for(int k=0; k<n; k++)
    for(int i=0; i<n; i++)
      for(int j=0; j<=n ;j++)
        thu.Calculate(i,k,j);
        
  int min=0xffffff;
  for(int i=0;i<n;i++)
    if(thu.node[i]->maxtime < min)
      min = thu.node[i] ->maxtime;
      
  std::printf("%d",min);
}
      