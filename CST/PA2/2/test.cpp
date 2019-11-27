#include<cstdio>
using namespace std;
typedef unsigned long long ll;
const int N = 500005, K = 64, B = 6, D = (K / B) + 1;
const int MASK = (1 << B) - 1, STA=K%B?K-K%B:K-B;

int n, k, e_siz;
char s[66];
int ans[N];
ll a[N];

struct edge {
  int nxt, t, id;
  edge() {}
  edge(int nxt, int t, int id):
    nxt(nxt), t(t), id(id){}
} e[N * D];

int head[N * D], siz[N * D], cid[N * D];

inline void adde(int x, int y, int id) {
  e_siz++;
  e[e_siz] = edge(head[x], y, id);
  head[x] = e_siz;
}

inline int find(int x, int id) {
  for(int k = head[x]; k; k = e[k].nxt)
    if(id == e[k].id)
      return e[k].t;
  return 0;
}

int trie_siz = 1, root = 1;

void insert(int x, ll val, int id, int d = STA) {
  siz[x]++;
  if(d < 0) {
    cid[x] = id;
    return;
  }
  int p = (val >> d)&MASK;
  int y = find(x, p);
  if(!y) {
    y = ++trie_siz;
    adde(x, y, p);
  }
  insert(y, val, id, d - B);
}

void del(int x, ll val, int d = STA) {
  siz[x]--;
  if(d < 0)return;
  int p = (val >> d)&MASK;
  int y = find(x, p);
  del(y, val, d - B);
}

int ask(int x, ll val, int d = STA) {
  if(d < 0)
    return cid[x];
  int p = (val >> d)&MASK;
  int y = 0, mx = 0,flag=0;
  for(int k = head[x]; k; k = e[k].nxt)
    if(siz[e[k].t] && (!flag || (e[k].id ^ p) > mx)) {
      flag=1;
      mx = e[k].id ^ p;
      y = e[k].t;
    }
  return ask(y,val,d-B);
}

inline int max(int a, int b){
  return a>b?a:b;
}

inline int min(int a, int b){
  return a<b?a:b;
}

int main() {
  scanf("%d%d", &n, &k);
  for(int i=0; i<n; i++){
    scanf("%s", s);
    ll x = 0;
    for(int i=0; i<K; i++)
      x |= ((ll)s[63 - i] - '0') << i;
    a[n-i-1] = x;
  }

  int lx = -1, rx = -1;
  for(int i=0; i<n; i++){
    int nlx = max(-1, i - k - 2);
    int nrx = min(n - 1, i + k + 1);
    while(nlx > lx) {
      lx++;
      del(root, a[lx]);
    }
    while(nrx > rx) {
      rx++;
      insert(root, a[rx], rx);
    }
    ans[i]=ask(root,a[i]);
  }
  int temp = n-ans[n-1]-1;
  if(temp){
    printf("%d\n", temp);
  }
  else{
    printf("1\n");
  }
  for(int i=1; i<n; i++)
    printf("%d\n",n-ans[n-i-1]-1);
}
