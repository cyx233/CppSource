#include<cstring>
#include<cstdio>
const int N = 1<<24;
char M[(N+7)>>3], input[(N+7)>>3];
void set(int k){ M[k>>3] |= 0x80>>(k & 7); }//标记k
bool test(int k){ return M[k>>3] & 0x80>>(k & 7); }//查找k是否被标记
int main(){
  char c;
  c = getchar();
  int tot=0;
  int cnt=0;
  while(c=='1' || c=='0'){
    if(c-'0')
      input[tot>>3] |= 0x80>>(tot & 7);//保存输入
    tot++;
    c = getchar();
  }
  for(int i=1; i<=24; i++){//枚举所有的答案长度
    cnt = 1<<i;//i位的答案最多有cnt种字符串
    memset(M, 0, cnt);
    for(int j = tot-i; j>=0; j--){//从第j位开始，向后取长i位的数
      int current = 0;
      for(int k=0; k<i; k++)
        if(input[(j+k)>>3] & 0x80>>((j+k) & 7))//若第j+k位为1
          current += 1<<(i-k-1);
      if(test(current))//若已标记
        continue;
      cnt--;//加入新标记
      if(!cnt)//第i位可能的答案都已经出现，直接开始枚举i+1位
        break;
      set(current);//标记current
    }
    if(cnt)//如果有可能答案没在输入中出现
      for(int j=0; j< 1<<i; j++)
        if(!test(j)){//查找最小的符合答案
          for(int k=i-1; k>=0; k--){
            printf("%d", (j>>k)&1);//二进制输出
          }
          printf("\n");
          return 0;
        }
  }
  return 0;
}
