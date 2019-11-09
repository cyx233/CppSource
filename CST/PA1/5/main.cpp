#include<cstdio>
#include<cstring>
#include<cmath>

char s[1500001];
int next[1500001];
int pre[1500001];
int len[1500001];
int size=1;
const int chunk = 3;
const int MIN_LEN = 500;
const int MAX_SIZE = 1500;

inline void rearrange();


inline void insert(int pos, char x){
  int begin = 0;
  while(begin+len[begin]-1<pos){
    if(next[begin]==-1)
      break;
    begin = next[begin];
    pos -= len[begin];
  }

  int move_len = len[begin] - pos;
  if(move_len<0)
    move_len=0;

  if(len[begin]<chunk){
    memmove(s+begin+pos+1, s+begin+pos, move_len);
    s[begin+pos] = x;
    len[begin]++;
  }
  else if(size < MAX_SIZE){
    if(begin != (size-1)*chunk){
      pre[next[begin]] = size*chunk;
      next[size*chunk] = next[begin];
    }
    else{
      next[size*chunk] = -1;
    }

    pre[size*chunk] = begin;
    next[begin] = size*chunk;

    size++;

    memmove(s+(size-1)*chunk, s+begin+pos, move_len);
    len[begin] = len[begin] - move_len + 1;
    len[(size-1)*chunk] = move_len;

    s[begin+pos] = x;
  }
  else{
    rearrange();
    insert(pos,x);
  }5
  if(len[begin]<MIN_LEN && size>1){
    int temp;
    int merge;
    if(pre[begin]!=-1){
      temp = len[begin];
      merge = pre[begin]+len[pre[begin]];
      if(len[begin] + temp < chunk){
        memmove(s+merge, s+begin, temp);
        len[begin] = len[begin] + temp;
        size--;
        next[pre[begin]] = next[begin];
        pre[next[begin]] = pre[begin];
      }

    }
    else{
      temp = len[next[begin]];
      merge = next[begin];
      if(len[begin] + temp < chunk){
        memmove(s+begin+len[begin], s+merge, temp);
        len[begin] = len[begin] + temp;
        size--;
        pre[next[next[begin]]] = begin;
        next[begin] = next[next[begin]];
      }
    }

  }
}

inline void remove(int index){
  int begin = (index/chunk)*chunk;
  if(index == begin){
    if(pre[begin]!=-1){
      next[pre[begin]] = next[begin];
    }
    if(next[begin]!=-1){
      pre[next[begin]] = pre[begin];
    }
    len[begin]=0;
  }
  else if(index == begin + len[begin] - 1){
    len[begin]--;
  }
  else{
    memmove(s+index, s+index+1, len[begin]-(index-begin+1));
    len[begin]--;
  }
}

inline bool check(int& pos){
  if(len[0]==0)
    return 0;
  int begin = 0;
  bool flag = 0;
  int temp = pos;
  while(len[begin]-1<temp){
    temp -= len[begin];
    begin = next[begin];
  }
  int start = pre[begin];
  int end = next[begin];
  if(start == -1)
    start = begin;
  if(end == -1)
    end = begin;
  int top=0;
  int stack[3*chunk];
  int aux[3*chunk];
  memset(stack, 0, sizeof(stack));
  memset(aux, 0, sizeof(aux));
  stack[0] = begin;
  aux[0] = 1;
  int current = start;

  int i=start + 1;
  if(len[current]==1){
    i = next[current];
    current = next[current];
  }
  while(1){
    if(i==end+len[end] || s[i] != s[stack[top]]){
      if(aux[top]>=3){
        flag = 1;
        if(stack[top]>=begin && stack[top]<pos)
          pos--;
        if(stack[top]>=start && stack[top]<start+len[start])
          pos--;
        while(top>=0){
          char temp = s[stack[top]];
          remove(stack[top]);
          if(len[end]==0){
            end = pre[end];
          }
          top--;
          i--;
          if(i<current){
            i = pre[current] + len[pre[current]] - 1;
            current --;
          }
          if(s[stack[top]]!=temp){
            break;
          }
        }
      }
    }
    if(i==-1 || i==end+len[end]){
      break;
    }
    if(s[i]==s[stack[top]])
      aux[top+1] = aux[top]+1;
    else{
      aux[top+1] = 1;
    }
    stack[top+1]=i;
    top++;

    i++;
    if(i>=current+len[current]){
      if(current != end){
        i = next[current];
        current = next[current];
      }
    }
  }
  return flag;
}

inline void rearrange(){
  if(size<=1)
    return;
  int begin=0;
  while(next[begin]!=-1){
    if(len[begin]+len[next[begin]]<chunk){
      memmove(s+begin+len[begin], s+next[begin], len[next[begin]]);
      len[begin] += len[next[begin]];
      if(next[next[begin]]!=-1)
        pre[next[next[begin]]] = begin;
      next[begin] = next[next[begin]];
      size--;
    }
  }
}

inline void print(){
  int begin = 0;
  while(begin!=-1){
    for(int i=begin; i<begin+len[begin]; i++){
      printf("%c", s[i]);
    }
    begin = next[begin];
  }
  printf("\n");
}

int main(){
  scanf("%s", s);
  int n=strlen(s);
  for(;size*chunk<n;size++){
    next[(size-1)*chunk] = size*chunk;
    pre[size*chunk] = (size-1)*chunk;
    len[(size-1)*chunk] = chunk;
  }
  pre[0]=-1;
  next[(size-1)*chunk] = -1;
  len[(size-1)*chunk] = n%chunk;
  if(len[(size-1)*chunk]==0)
    len[(size-1)*chunk] = chunk;

  int m;
  scanf("%d",&m);
  for(int i=0; i<m; i++){
    char color[2];
    int pos;
    scanf("%d%s", &pos, &color);
    insert(pos, color[0]);
    while(check(pos)){
      rearrange();
    }
    print();
  }
  return 0;
}
