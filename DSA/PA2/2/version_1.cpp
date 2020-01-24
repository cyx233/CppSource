#include<cstdio>
#include<cstring>
#include<cstdlib>
typedef long long ll;
const int INF=1<<29,N=32000001,len=64;
using namespace std;


int cnt=1, a[N][2], sum[N], id[N];

void insert(int x, int &y, char val[], int d, int data){
  y = cnt;
  cnt++;
  sum[y] = sum[x] + 1;
  if(d < 0){
    id[y] = data;
    return;
  }
  int p = val[63-d]-'0';
  a[y][p^1] = a[x][p^1];
  insert(a[x][p], a[y][p], val, d-1, data);
}

int query(int x, int y, char val[], int d){
  if(d < 0){
    return id[y];
  }
  int p = val[63-d] - '0';
  if(sum[a[y][p^1]] - sum[a[x][p^1]]){
    return query(a[x][p^1], a[y][p^1], val, d-1);
  }
  return query(a[x][p], a[y][p], val, d-1);
}

inline int max(int a, int b){
  return a>b?a:b;
}

inline int min(int a, int b){
  return a>b?b:a;
}

int n,m,b[N],root[50001];
int main(){
  int n,k;
  scanf("%d%d",&n,&k);
  char a[50001][100];
  for(int i=1;i<=n;i++){
    scanf("%s",a[i]);
    insert(root[i-1], root[i], a[i], len-1, i-1);
  }
  for(int i=1;i<=n;i++){
    int l = max(i-k-2, 0);
    int r = min(i+k+1, n);
    printf("%d\n", query(root[l], root[r], a[i], len-1));
  }
  return 0;
}
