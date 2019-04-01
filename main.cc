#include<cstdio>
#include<ctime>
#include<cstdlib>
#include"node.h"
#include"edge.h"
#include"graph.h"

int list[50][2];
Graph graph;

void generate();
void init();
void calc();
void print();

int main(){
  generate();
  init();  
  graph.Sort();
  calc();
  print();
  return 0;
}  


void generate(){
  srand(time(NULL));
  int num = 0, x, y;
  while(num < 50){
    x = rand()%10001;
    y = rand()%10001;
    while(1){
      int j;
      for(j=0; j<num; j++){
        if(x == list[j][1] && y == list[j][2]){
          x = rand()%10001;
          y = rand()%10001;
          break;
        }
      }
     if(j == num)
       break;
    }
    list[num][1] = x;
    list[num][2] = y;
  }
}


void init(){
  for(int i=0; i<50; i++)
    graph.AddNode(Node(i, list[i][0], list[i][1]));
  int edge_num = 0;
  for(int i=0; i<50; i++)
    for(int j=0; j<i; j++){
      graph.AddEdge(Edge(edge_num, graph.node(i), graph.node(j))); 
      edge_num++;
    }
}

void calc(){
   
}
