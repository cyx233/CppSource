#include <cstdio>
typedef long long int ll;
struct SegmentTreeNode{
  int l;
  int r;
  ll k;
  int lazy;
  SegmentTreeNode* left;
  SegmentTreeNode* right;
  SegmentTreeNode(int ml, int mr, ll mk, int mlazy=0,
      SegmentTreeNode* mleft=nullptr, SegmentTreeNode* mright=nullptr):
    l(ml), r(mr), k(mk), lazy(mlazy), left(mleft), right(mright){}
};

void normalize(SegmentTreeNode* node){
  if(node->lazy > 0)
    node->k += node->lazy * (node->r - node->l + 1);
  if(node->l < node->r){
    if(!node->left || !node->right){
      int mid = ((ll)node->l + (ll)node->r)/(ll)2;
      node->left = new SegmentTreeNode(node->l, mid, 0);
      node->right = new SegmentTreeNode(mid + 1, node->r, 0);
    }
    node->left->lazy += node->lazy;
    node->right->lazy += node->lazy;
  }
  node->lazy = 0;
}

ll search(SegmentTreeNode* node, int i, int j){
  normalize(node);
  if(i>j || !node || i>node->r || j<node->l)
    return 0;
  if(node->l>=i && node->r <= j)
    return node->k;
  return search(node->left, i, j) + search(node->right, i, j);
}

void add(SegmentTreeNode* node, int i, int j){
  normalize(node);
  if(i>j || !node || i>node->r || j<node->l)
    return;
  if(node->l>=i && node->r <= j){
    node->lazy = 1;
    normalize(node);
    return;
  }
  add(node->left, i, j);
  add(node->right, i, j);
  node->k = node->left->k + node->right->k;
}



int main(){
  int n;
  int m;
  scanf("%d%d", &n, &m);
  SegmentTreeNode* root = new SegmentTreeNode(1, n, 0);
  for(int i=0; i<m; i++){
    char c[5];
    scanf("%s",c);
    if(c[0]=='H'){
      int tl, tr;
      scanf("%d%d", &tl, &tr);
      add(root,tl,tr);
    }
    if(c[0]=='Q'){
      int tl, tr;
      scanf("%d%d", &tl, &tr);
      printf("%lld\n",search(root,tl,tr));
    }
  }
  return 0;
}
