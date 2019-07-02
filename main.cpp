#include<string>
using std::string;
using std::to_string;
class Solution {
 public:
  int reverse(int x) {       
    int neg = 0;
    if(x<0)
      neg = 1;
    string a = to_string(x);
    if(neg)
      a.assign(a,1,a.size()-1);
    int len = a.size();
    int sum = 0;
    for(int i=len-1;i<=0;i--){
      sum = sum*10 + (a[i]-'0');
    }
    return sum;
  }
};
