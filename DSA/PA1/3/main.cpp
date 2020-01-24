#include<cstdio>
#include<iostream>
#include<cstring>

using namespace std;

int min_add(int i,int j,int num1,int num2){//从d[i][j]到终点d[num1][num2]可能的最小的代价
  int aux = num1-num2+j-i;
  if(aux<0)
    aux = -aux;
  return aux;
}

char a[501001], b[501001];
int d[501001];

int main(){
  int num1, num2, max_cost;
  scanf("%d%d%d", &num1, &num2, &max_cost);

  if(num1-num2>max_cost || num1-num2<-max_cost){//超过最大代价
    printf("%d",-1);
    return 0;
  }
  int auxi = 0;

  while(auxi<=num1 && auxi<=max_cost){//判断首个超过最大代价的d[i][0]
    if(auxi + min_add(auxi,0,num1,num2) > max_cost)
      break;
    auxi++;
  }

  scanf("%s", a);
  scanf("%s", b);

  for(int i=0;i<=num2;i++)
    d[i] = i;

  int temp;
  for(int i=0;i<num1;i++){
    int min_result = d[0] + min_add(i,0,num1,num2);
    int begin=0;

    if(min_result > max_cost){//d[i][0]超过代价则从首个可能不超过的d[i][begin]开始
      begin = i-auxi+1;
    }

    temp = d[begin];
    d[0] = i+1;

    for(int j=begin;j<num2;j++){//编辑距离
      if(a[i] == b[j]){
        int t = temp;
        temp = d[j+1];
        d[j+1] = t;
      }
      else{
        temp = d[j+1];
        int min_result_up = d[j+1] + min_add(i-1,j+1,num1,num2);
        int min_result_left = d[j] + min_add(i,j,num1,num2);
        if(min_result_up > max_cost){
          d[j+1] = d[j]+1;
        }
        else if(min_result_left > max_cost){
          d[j+1] = d[j+1]+1;
        }
        else{
          d[j+1] = min(d[j], d[j+1])+1;
        }
      }

      min_result = d[j+1] + min_add(i+1,j+1,num1,num2);
      if(min_result > max_cost && j+num1-i>num2){//若超过第i行右边界,直接进行i+1行的迭代
        break;
      }
    }
    /*for(int j=0; j<=num2;j++){
      cout<<d[j]<<" ";
      }
      cout<<endl;*/
  }

  if(d[num2]<=max_cost){//输出
    printf("%d\n", d[num2]);
  }
  else{
    printf("%d\n",-1);
  }
  return 0;
}
