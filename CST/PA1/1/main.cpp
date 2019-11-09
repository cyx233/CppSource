#include<iostream>
#include<cstdio>
#include<cmath>
#include<cstring>
#include<string>
using namespace std;
typedef long long ll;

class complex{//复数的实现
  public:
    complex():real(0),im(0){}
    complex(double a, double b):real(a), im(b){}

    inline complex operator * (const complex& a)const{//multiple for complex*complex
      complex temp;
      temp.real = this->real * a.real - this->im * a.im;
      temp.im = this->real * a.im + this->im * a.real;
      return temp;
    }
    inline complex operator + (const complex& a){//plus for complex+complex
      complex temp;
      temp.real = this->real + a.real;
      temp.im = this->im + a.im;
      return temp;
    }

    inline complex operator - (const complex& a){//minus for complex-complex
      complex temp;
      temp.real = this->real - a.real;
      temp.im = this->im - a.im;
      return temp;
    }

    inline complex operator / (const double a){//divide for complex/double
      complex temp;
      temp.real = this->real/a;
      temp.im = this->im/a;
      return temp;
    }
    double real;//实部
    double im;//虚部
};

inline complex conj(complex a){//共轭复数
  return complex(a.real, -a.im);
}

const int MAX = 17000;//最大所需数组空间
const int maxn = 16384;//多项式最大次数,2^14 > 5000*2 - 1
const int yawei = 4;//压4位, 即10000进制
const double PI = acos(-1);//3.14159....

class FFT{//快速傅里叶变换
  public:
    complex w[MAX];//单位根
    complex w_conj[MAX];//单位根的倒数

    inline void init(int n){
      for(int i=0; i<=n; i++) {
        w[i] = complex(cos(2*PI/(double)n*i), sin(2*PI/(double)n*i)); //单位根
        w_conj[i] = conj(w[i]); //单位根的倒数
      }
    }

    inline void FastF(complex*num, const int tot, const complex* mw){
      //快速傅里叶,首先系数以实部的形式保存在num数组,
      //函数结束时num数组变为tot个复数表示的点值
      int wei_shu = 0;//二进制位数
      while(1<<wei_shu < tot)
        wei_shu++;
      for(int i=0; i<tot; i++){
        int temp=0;
        for(int j=0;j<=wei_shu;j++){
          if(i & 1<<j){//若i在第j位是1
            temp |= 1<<(wei_shu-j-1);//t在倒数第j位变为1
          }
        }
        if(i<temp)
          swap(num[i], num[temp]);//i和t为二进制倒转的两数,
        //调转两个位置的数即可得到分治边界(观察得出),此部分思路来源于博客
      }

      for(int bigger=2; bigger<=tot; bigger <<= 1){//两个smaller长度的分治子项合成bigger
        int smaller = bigger/2;
        for(int i=0; i<tot; i += bigger){
          complex* p=num + i;//蝴蝶操作,仅需一维数组即可进行迭代（动态规划时有类似做法）
          for(int j=0; j<smaller; j++){
            complex temp = mw[maxn/bigger*j]*p[smaller+j];
            p[smaller+j] = p[j] - temp;//报告中迭代式2
            p[j] = p[j] + temp;//报告中迭代式1
          }
        }
      }
    }

    inline void num2point(complex* a, const int tot){//系数式->点值式
      FastF(a, tot, w);
    }

    inline void point2num(complex* a, const int tot){//点值式->系数式
      FastF(a, tot, w_conj);
      for(int i=0; i<tot; i++)
        a[i] = a[i]/tot;//逆向傅里叶变换
    }

}fft;
inline void multiple(const int* a1, const int* a2, const int n1, const int n2, ll* res){
  //由系数数组a1，与系数数组a2表示的两个多项式相乘,函数结束后运算结果在系数数组res
  int n=1;
  while(n<n1+n2)
    n<<=1;//获得乘法结果的最大可能位数
  complex num1[MAX], num2[MAX];//两个用来表示多项式的数组
  for(int i=0; i<n1; i++){
    num1[i].real = a1[i];//将a1置入num1
  }
  for(int i=0; i<n2; i++){
    num2[i].real = a2[i];//将a2置入num2
  }

  fft.num2point(num1, n);//将系数式num1表示为n个点值(复数)
  fft.num2point(num2, n);//将系数式num2表示为n个点值(复数)
  for(int i=0;i<n;i++){
    num1[i] = num1[i] * num2[i];//多项式相乘即为点值(复数)相乘,获得n个点值储存在num1
  }
  fft.point2num(num1,n);//储存于num1中的n个点值变成系数储存于num1
  for (int i=0; i<n1+n2-1; i++)
    res[i] = static_cast<ll>(floor(num1[i].real + 0.5));//由于整数乘法,系数一定为整型
  ll jinwei = 0;//处理进位
  for(int i=0; i<n1+n2-1; i++){
    res[i] = jinwei + res[i];//10000进制处理进位
    jinwei = res[i]/10000;
    res[i] = res[i]%10000;
  }
  res[n1+n2-1]=jinwei;//最高位
}

int main(){
  int tot = 0;//乘法次数
  scanf("%d",&tot);
  ll res[MAX];//乘法结果
  int num1[MAX],num2[MAX];//乘数1,乘数2
  memset(res,0,sizeof(res));
  memset(num1,0,sizeof(num1));
  memset(num2,0,sizeof(num2));
  int n1,n2;//位数1,位数2
  char a[MAX]={'\0'};
  char b[MAX]={'\0'};
  fft.init(maxn);//初始化单位根

  for(int i=0; i<tot; i++){
    memset(a,'\0',sizeof(a));
    memset(b,'\0',sizeof(b));
    scanf("%s",a);
    scanf("%s",b);
    n1 = strlen(a);
    int temp=0;
    for(int j=n1-1;j>=0;j-=yawei){//10000进制,每4位为一运算单元
      int sum = 0;
      int begin = max(j-yawei+1,0);
      for(int k=begin; k<=j;k++){
        sum = sum*10 + a[k] - '0';
      }
      num1[temp] = sum;
      temp++;
    }

    if(n1%yawei==0)//得到10000进制数的位数
      n1 = n1/yawei;
    else
      n1 = n1/yawei +1;

    temp=0;//此部分同上
    n2 = strlen(b);
    for(int j=n2-1;j>=0;j-=yawei){
      int sum = 0;
      int begin = max(j-yawei+1,0);
      for(int k=begin; k<=j;k++){
        sum = sum*10 + b[k] - '0';
      }
      num2[temp] = sum;
      temp++;
    }

    if(n2%yawei==0)
      n2 = n2/yawei;
    else
      n2 = n2/yawei +1;

    multiple(num1, num2, n1, n2, res);//乘法运算

    int begin = n1+n2-1;
    while(res[begin]==0){//去掉首0
      if(begin == 0){
        cout<<"0";
        begin--;
        break;
      }
      begin--;
    }

    int flag = 1;
    for(int k=begin; k>=0; k--){//打印输出,从最高位开始
      if(flag==1){
        cout<<res[k];
        flag=0;
      }
      else if(res[k]>=1000){
        cout<<res[k];
      }
      else if(res[k]>=100){
        cout<<"0"<<res[k];
      }
      else if(res[k]<100 && res[k]>=10){
        cout<<"00"<<res[k];
      }
      else if(res[k]<10){
        cout<<"000"<<res[k];
      }
    }
    cout<<endl;
  }
  return 0;
}

