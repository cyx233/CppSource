#include<cstdio>
#include<iostream>
#include<cstdlib>
using namespace std;
typedef long long int ll;

class Graph{//坐标系的类
  public:
    int line_x[200001];//x轴上的点
    int line_y[200001];//y轴上的点
    int num;//点的个数
    int find(int x, int y);//求问题中所需交点个数(x,y)
    int Directed_S(int y1, int x2, int x3, int y3);//计算三角形有向面积
}graph;

inline int Graph::Directed_S(int y1, int x2, int x3, int y3){
  ll temp1 = (ll)x2*(ll)(y3-y1), temp2 = (ll)x3*(ll)y1;//longlong储存两个大整数

  if(temp1>0 && temp2>0){//有向面积为正,1代表
    return 1;
  }
  else if(temp1<0 && temp2<0){//有向面积为负,-1代表
    return -1;
  }
  else if(temp1+temp2<0){
    return -1;
  }
  else if(temp1+temp2>0){
    return 1;
  }
  else{//有向面积为0,0代表
    return 0;
  }
}

inline int Graph::find(int x,int y){
  int l=0;
  int r=num-1;
  int mid = (r+l)/2;
  if(Directed_S(line_y[l],line_x[l],x,y)<0)//判断左边界
    return 0;
  if(Directed_S(line_y[r],line_x[r],x,y)>=0)//判断右边界
    return num;
  while(l<r-1){//二分查找
    if(Directed_S(line_y[mid],line_x[mid],x,y)>=0){//mid位置的线段在点的左侧
      l = mid;
      mid = (l+r)/2;
    }
    else{//mid位置的线段在点的右侧
      r = mid;
      mid = (l+r)/2;
    }
  }
  return l+1;
}

int cmp(const void* x, const void* y){//排序比较函数
  return *(int*)x - *(int*)y;
}

int main(){
  //读入数据
  scanf("%d", &graph.num);
  for(int i=0;i<graph.num;i++){
    scanf("%d",&graph.line_x[i]);
  }
  for(int i=0;i<graph.num;i++){
    scanf("%d",&graph.line_y[i]);
  }

  //数据排序
  qsort(graph.line_x,graph.num,sizeof(*graph.line_x),cmp);
  qsort(graph.line_y,graph.num,sizeof(*graph.line_y),cmp);

  //读入询问并计算
  int qustion_num;
  scanf("%d",&qustion_num);
  for(int i=0; i<qustion_num; i++){
    int x,y;
    scanf("%d%d",&x,&y);
    printf("%d\n",graph.find(x,y));
  }
  return 0;
}

