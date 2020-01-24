#include <cstdio>
#include <cstring>

char s[8000001];//无向表
int neighbor1[8000001];//邻点的下标
int neighbor2[8000001];//另一个邻点的下标
const int MAX_SIZE = 8000000;//最多节点个数
int cl,cr;//左指针，右指针
int end;//尾指针'\0'
int tail;//新生成的数据位置
int aheadl;//tracel->cl的朝向, 1为向后
int aheadr;//tracer->cr的朝向, 1为向后
int tracel;//左指针的某一邻点
int tracer;//右指针的某一邻点
int distance;//左右指针距离. 右指针-左指针, 当重合时为-1


//查找pos点的前一个点, trace为某个相邻点, ahead为trace->pos方向,1为向后
inline int pre(int pos, int trace, int ahead){
  int pre;
  if(ahead==0){//trace->pos向前, 即trace为后节点
    if(neighbor2[pos]==trace){
      pre = neighbor1[pos];
    }
    else{
      pre = neighbor2[pos];
    }
  }
  else{//trace为前节点
    if(neighbor2[pos]==trace){
      pre = neighbor2[pos];
    }
    else{
      pre = neighbor1[pos];
    }
  }
  return pre;
}

//查找pos点的后一个点, trace为某个相邻点, ahead为trace->pos方向,1为向后
inline int next(int pos, int trace, int ahead){
  int preid = pre(pos, trace, ahead);//先获取pos的前节点,另一邻点即为后节点
  if(neighbor1[pos] == preid){
    return neighbor2[pos];
  }
  else{
    return neighbor1[pos];
  }
}


inline void move_left(char aux){//向左(前)移动
  if(aux=='L'){
    int preid = pre(cl, tracel, aheadl);
    if(preid == 0){
      printf("F\n");
      return;
    }
    tracel = cl;
    aheadl = 0;
    cl = preid;
    distance++;//左指针左移, 下标差+1
  }
  else{
    if(cr == 0){
      printf("F\n");
      return;
    }
    int preid = pre(cr, tracer, aheadr);
    tracer = cr;
    aheadr = 0;
    cr = preid;
    distance--;//右指针左移, 下标差-1
  }
  printf("T\n");
}

inline void move_right(char aux){//向右(后)移动
  if(aux=='L'){
    if(cl == end){
      printf("F\n");
      return;
    }
    int nextid = next(cl, tracel, aheadl);
    tracel = cl;
    aheadl = 1;
    cl = nextid;
    distance--;//左指针右移, 下标差-1
  }
  else{
    int nextid = next(cr, tracer, aheadr);
    if(nextid == end){
      printf("F\n");
      return;
    }
    tracer = cr;
    aheadr = 1;
    cr = nextid;
    distance++;//右指针右移, 下标差+1
  }
  printf("T\n");
}


inline void cl_insert(char aux){//左指针插入
  int preid = pre(cl, tracel, aheadl);//当前节点与前节点之间插入
  int add = tail;//获取插入位置
  tail++;

  //插入
  neighbor1[add] = preid;
  neighbor2[add] = cl;

  if(neighbor1[preid] == cl){
    neighbor1[preid] = add;
  }
  else{
    neighbor2[preid] = add;
  }

  if(neighbor1[cl] == preid){
    neighbor1[cl] = add;
  }
  else{
    neighbor2[cl] = add;
  }
  //


  //更改和指针有关的标志
  if(tracer == preid && cr==cl){
    tracer = add;
  }

  if(tracel == preid){
    tracel = add;
  }

  if(distance == -1){
    tracer = preid;
    aheadr = 1;
    cr = add;
  }
  //

  //若右指针在左指针左边, 坐标差-1(相距更大)
  if(distance < -1){
    distance--;
  }

  s[add] = aux;
  printf("T\n");
}

inline void cr_insert(char aux){//右指针插入
  int nextid = next(cr, tracer, aheadr);//在当前节点与后节点之间插入
  int add = tail;//获取插入位置
  tail++;

  //插入
  neighbor1[add] = cr;
  neighbor2[add] = nextid;
  if(neighbor1[nextid] == cr){
    neighbor1[nextid] = add;
  }
  else{
    neighbor2[nextid] = add;
  }

  if(neighbor1[cr] == nextid){
    neighbor1[cr] = add;
  }
  else{
    neighbor2[cr] = add;
  }
  //


  //更改和指针有关的标志
  if(tracel == nextid && cl == cr){
    tracel = add;
  }

  tracer = cr;
  cr = add;
  aheadr = 1;

  if(distance == -1){
    tracer = add;
    aheadr = 1;
  }
  //

  //若左指针在右指针左边, 坐标差+1
  if(distance > -1){
    distance++;
  }

  s[add] = aux;
  printf("T\n");
}

inline void cl_delete(){//左指针删除
  if(cl==end){
    printf("F\n");
    return;
  }

  //删除当前节点, 前后节点相连
  int nextid = next(cl, tracel, aheadl);
  int preid = pre(cl, tracel, aheadl);

  //删除jj
  if(neighbor1[preid]==cl){
    neighbor1[preid]=nextid;
  }
  else{
    neighbor2[preid]=nextid;
  }

  if(neighbor1[nextid]==cl){
    neighbor1[nextid]=preid;
  }
  else{
    neighbor2[nextid]=preid;
  }
  //


  //更改和指针有关的标志
  if(tracer == cl){
    if(preid == cr){
      tracer = nextid;
      aheadr = 0;
    }
    else{
      tracer = preid;
      aheadr = 1;
    }
  }

  if(cl == cr){
    cr = preid;
    tracer = nextid;
    aheadr = 0;
  }

  cl = nextid;
  tracel = preid;
  aheadl = 1;
  //

  //若左指针在右指针左边, 坐标差-1
  if(distance > -1){
    distance--;
  }


  printf("T\n");
}

inline void cr_delete(){//删除右指针
  //删除后节点
  int nextid = next(cr, tracer, aheadr);
  if(nextid==end){
    printf("F\n");
    return;
  }

  //当前节点与后节点的后节点相连
  int nnid = next(nextid, cr, 1);

  //删除
  if(neighbor1[cr] == nextid){
    neighbor1[cr] = nnid;
  }
  else{
    neighbor2[cr] = nnid;
  }

  if(neighbor1[nnid] == nextid){
    neighbor1[nnid] = cr;
  }
  else{
    neighbor2[nnid] = cr;
  }
  //

  //更改和指针有关的标志
  if(tracel == nextid){
    if(cl == cr){
      tracel = nnid;
      aheadl = 0;
    }
    else{
      tracel = cr;
      aheadl = 1;
    }
  }

  if(cl == nextid){
    cl = nnid;
    tracel = cr;
    aheadl = 1;
  }

  if(tracer == nextid){
    tracer = nnid;
    aheadr = 0;
  }
  //

  //若左指针在右指针右边, 坐标差-1
  if(distance < -1){
    distance ++;
  }


  printf("T\n");
}


inline void revers(){//倒转

  if(distance <= -1){//若左指针不在右指针左边, 失败
    printf("F\n");
    return;
  }

  //倒转连接
  int preid = pre(cl, tracel, aheadl);
  int nextid = next(cr, tracer, aheadr);

  if(neighbor1[preid] == cl){
    neighbor1[preid] = cr;
  }
  else{
    neighbor2[preid] = cr;
  }
  if(neighbor1[nextid] == cr){
    neighbor1[nextid] = cl;
  }
  else{
    neighbor2[nextid] = cl;
  }

  if(neighbor1[cl] == preid){
    neighbor1[cl] = nextid;
  }
  else{
    neighbor2[cl] = nextid;
  }

  if(neighbor1[cr] == nextid){
    neighbor1[cr] = preid;
  }
  else{
    neighbor2[cr] = preid;
  }
  //

  //更改和指针有关的标志
  int temp = cl;
  cl = cr;
  cr = temp;

  tracel = preid;
  tracer = nextid;

  aheadl = 1;
  aheadr = 0;
  //

  printf("T\n");
}

inline void show(){//打印整个字符串
  int show_p=neighbor2[0];//从第一个字符开始
  int show_trace=0;
  int pos = 0;
  char ans[8000001];
  while(show_p!=end){
    ans[pos] = s[show_p];

    //指针后移一位
    int temp = next(show_p, show_trace, 1);
    show_trace = show_p;
    show_p = temp;
    pos++;
    //
  }
  ans[pos] = '\0';
  printf("%s\n", ans);//只调用一次printf(), 常数优化
}

int main(){
  //初始化
  scanf("%s", s+1);
  s[0]='h';
  cl = 1;
  tracel = 0;
  aheadl = 1;

  cr = strlen(s)-1;
  end = cr+1;
  tail = end+1;
  tracer = end;
  aheadr = 0;
  distance = cr - cl;
  //

  //进行操作
  for(int i=0; i<=end; i++){
    neighbor1[i] = i-1;
    neighbor2[i] = i+1;
  }
  neighbor2[0] = 1;
  neighbor2[end] = -1;
  int num;
  scanf("%d", &num);
  for(int i=0; i<num; i++){
    char command[2];
    char aux1[2];
    char aux2[2];
    scanf("%s", command);
    if(command[0] == '<'){
      scanf("%s", aux1);
      move_left(aux1[0]);
    }
    else if(command[0] == '>'){
      scanf("%s", aux1);
      move_right(aux1[0]);
    }
    else if(command[0] == 'I'){
      scanf("%s", aux1);
      scanf("%s", aux2);
      if(aux1[0]=='L')
        cl_insert(aux2[0]);
      else{
        cr_insert(aux2[0]);
      }
    }
    else if(command[0] == 'D'){
      scanf("%s", aux1);
      if(aux1[0]=='L')
        cl_delete();
      else{
        cr_delete();
      }
    }
    else if(command[0] == 'R'){
      revers();
    }
    else if(command[0] == 'S'){
      show();
    }
  }
  //
  return 0;
}
