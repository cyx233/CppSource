int min_list[1000001];//单调队列
int node[1000001][3];//存储数据
long long int d[10000001];

const long long int MAX = 0x733fffffffffffff;

#include<cstdio>

int main(){
  int num;//读入数据
  scanf("%d", &num);
  for(int i=1;i<=num-1;i++){
    scanf("%d%d%d",&node[i][0], &node[i][1], &node[i][2]);
  }
  int pre_t=0,pre_h=0;//单调队列tail, head
  int min_t=0,min_h=0;//前继区间tail, head
  pre_t = 1;//推入起点
  min_t=1;
  min_list[1] = 1;
  for(int i=2; i<=num; i++){//从第二个点开始
    int head = pre_h;

    //寻找前继区间第一个点
    for(;head<i-1 && !(node[head+1][0]<=i && node[head+1][1]>=i); head++);

    if(head == i-1){//没有前继
      d[i] = MAX;
      continue;
    }

    int tail = pre_t+1>head+2? pre_t+1:head+2;

    //寻找前继区间最后一个点
    for(;tail<=i-1 && node[tail][0]<=i && node[tail][1]>=i; tail++);

    //若前继区间不空
    for(int j=pre_h; j<head; j++){//对出队的点维护单调队列
      int id = min_list[min_h+1];
      if(j+1==id)
        min_h++;
    }

    for(int j=pre_t+1; j<=tail-1; j++){//对入队的点维护单调队列
      while(min_h<min_t){
        int id=min_list[min_t];
        if((long long int)node[id][2]+d[id] >= (long long int)node[j][2]+d[j]){
          min_t--;
        }
        else
          break;
      }
      min_list[min_t+1] = j;
      min_t++;
    }

    pre_t = tail-1;//滑动窗口
    pre_h = head;

    int id = min_list[min_h+1];
    long long int temp = (long long int)node[id][2] + d[id];
    if(d[i]==0){//第一次访问i
      d[i] = temp;
    }
    else{
      d[i] = d[i]<temp? d[i]:temp;
    }

  }
  printf("%lld\n",d[num]);
}
