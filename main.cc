#include<cstdio>
#include<ctime>
#include<cstdlib>
#include"node.h"
#include"edge.h"
#include"graph.h"

int list[50][2];
Graph graph;
Graph mst;

void Generate();
void Init();
bool Cycle(int i);
bool Calc(int i);
void Print();

int main(){
  Generate();//生产随机数
  Init();//读入数据,初始化图
  graph.Sort();//以边长排序

  //Kruskal算法 
  int i=0;
  while(!Calc(i))
    i++;
    
  //打印输出
  Print();
  return 0;
}  


void generate(){
  srand(time(NULL));
  int num = 0, x, y;
  while(num < 50){
    x = rand()%10001;
    y = rand()%10001;

    //去重
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
  for(int i=0; i<50; i++){
    graph.AddNode(Node(i, list[i][0], list[i][1]));
    mst.AddNode(Node(i, list[i][0], list[i][1]));
  }
  int edge_num = 0;

  //遍历完全图
  for(int i=0; i<50; i++)
    for(int j=0; j<i; j++){
      graph.AddEdge(Edge(edge_num, graph.node(i), graph.node(j))); 
      edge_num++;
    }
}

bool Calc(int i){
  Edge curedge = graph.edge(i);
  
  //判断算法终点
  if(mst.edge_num() == graph.node_num() - 1)
    return true;
  
  //判断新增边是否形成回路
  if(mst.IsCycle(curedge))
    return false;
  else
    mst.AddEdge(curedge);
  return false;
}

void Print(){
  float sum = 0;
  
  //打印边权和
  for(int i=0; i<mst.edge_num(); i++){
    sum += mst.edge(i).dis(); 
  }
  printf("最小生成树:%f\n", sum);

  //打印边列表
  for(int i=0; i<mst.edge_num(); i++){
    printf("edge%d:\n", i);
    printf("x1 = %d y1 = %d;\n", 
           mst.edge(i).n1().x(), mst.edge(i).n1().y()); 
    printf("x1 = %d y1 = %d;\n", 
           mst.edge(i).n2().x(), mst.edge(i).n2().y()); 
  }
  return; 
}
