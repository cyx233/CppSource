#include "sort.h"
#include <cstdio>

inline int max(int a, int b) { return a > b ? a : b; }
inline int min(int a, int b) { return a < b ? a : b; }
int temp[1000010]; //归并排序使用的临时数组

void merge2(int arr[], int tmp[], int l[], int r[], int target) //二路归并
/*******************************************************************
二路归并
arr是原始序列，tmp是保存归并结果的序列
二个子序列为[l[0], r[0]), [l[1], r[1])
target是目前归并到的位置，[0, target)有序
********************************************************************/
{
  int extra = 0, other = 1, a_max, tmp_cnt = target;
  int p[2] = { l[0], l[1] };
  if (l[0] + 1 < r[0]) {        //若0序列有多个元素
    extra = 0;                  // 0序列提供2个进行比较
    other = 1;                  // 1序列提供1个进行比较
  } else if (l[1] + 1 < r[1]) { //同上
    extra = 1;
    other = 0;
  } else { //若两个序列都只有一个元素，直接获得排序结果
    compare(arr[p[0]], arr[p[1]], arr[p[1]], &tmp[tmp_cnt + 1], &tmp[tmp_cnt]);
    return;
  }
  while (1) { //进行归并
    // extra序列多提供了2个元素进行比较
    compare(arr[p[0]], arr[p[1]], arr[p[extra] + 1], &a_max, &tmp[tmp_cnt++]);
    if (arr[p[other]] == a_max) { //若other序列的首个元素比extra的2个都大
      tmp[tmp_cnt++] = arr[p[extra] + 1];
      p[extra] += 2;                    // extra一次入列两个元素
      if (p[extra] + 1 == r[extra]) {   //若extra剩余1个元素
        if (p[other] + 1 == r[other]) { //若other也只剩了1个元素，直接完成归并
          compare(arr[p[0]], arr[p[1]], arr[p[1]], &tmp[tmp_cnt + 1], &tmp[tmp_cnt]);
          return;
        }
        other = !other; //若other还有多个元素，交换other和extra
        extra = !extra;
      } else if (p[extra] == r[extra]) { //若extra序列已经空
        for (int i = p[other]; i < r[other]; i++)
          tmp[tmp_cnt++] = arr[i]; // other序列剩余元素直接归并
        return;
      }
    } else if (arr[p[other]] == tmp[tmp_cnt - 1]) { //若other序列的首个元素是最小值
      ++p[other];                                   //最小值入列
      if (p[other] == r[other]) {                   //若other序列已空
        for (int i = p[extra]; i < r[extra]; i++)
          tmp[tmp_cnt++] = arr[i]; // extra序列剩余元素直接归并
        return;
      }
    } else {                          //若other序列首个元素是3个比较元素的中间值
      p[extra]++;                     //说明extra序列首个元素是最小值，入列
      tmp[tmp_cnt++] = arr[p[other]]; //同时说明other序列首个元素比不过extra序列第二个元素
      p[other]++;                     // other序列首个元素入列
      if (p[other] == r[other]) {     //若other序列已空
        for (int i = p[extra]; i < r[extra]; i++)
          tmp[tmp_cnt++] = arr[i]; // extra序列剩余元素直接归并
        return;
      }
      if (p[extra] + 1 == r[extra]) {   //若extra序列剩余一个元素
        if (p[other] + 1 == r[other]) { //若other序列也只剩一个元素，直接完成归并
          compare(arr[p[0]], arr[p[1]], arr[p[1]], &tmp[tmp_cnt + 1], &tmp[tmp_cnt]);
          return;
        }
        other = !other; //若other序列还剩多个元素，交换other和extra
        extra = !extra;
      }
    }
  }
}
void merge3(int arr[], int tmp[], int l[], int r[], int target)
/*******************************************************************
三路归并
arr是原始序列，tmp是保存归并结果的序列
三个子序列为[l[0], r[0]), [l[1], r[1]), [l[2], r[2])
target是目前归并到的位置，[0, target)有序
********************************************************************/
{
  int p[3] = { l[0], l[1], l[2] };
  int a_max;
  int tmp_cnt = target;
  while (1) { //每次比较入列最小值
    compare(arr[p[0]], arr[p[1]], arr[p[2]], &a_max, &tmp[tmp_cnt++]);
    for (int i = 0; i < 3; i++) {
      if (arr[p[i]] == tmp[tmp_cnt - 1]) //若i序列为最小值，i序列入列
        p[i]++;
      if (p[i] == r[i]) { //若i序列已空
        int cl[2], cr[2], count = 0;
        for (int j = 0; j < 3; j++) {
          if (j != i) {
            cl[count] = p[j];
            cr[count] = r[j];
            count++;
          }
        }
        merge2(arr, tmp, cl, cr, tmp_cnt); //退化为剩余两个序列的二路归并
        return;
      }
    }
  }
}

void merge4(int arr[], int tmp[], int l[], int r[], int target)
/*******************************************************************
四路归并
arr是原始序列，tmp是保存归并结果的序列
四个子序列为[l[0], r[0]), [l[1], r[1]), [l[2], r[2]), [l[3], r[3])
target是目前归并到的位置，[0, target)有序
********************************************************************/
{
  int p[4] = { l[0], l[1], l[2], l[3] };
  int a_max, a_min;
  int tmp_cnt = target;
  int cmp_cnt = 0;
  int cmp[3] = { arr[p[0]], arr[p[1]], arr[p[2]] }; //参与排序的3路，第一次随意选择
  compare(cmp[0], cmp[1], cmp[2], &a_max, &a_min);
  for (int i = 0; i < 4; i++) { //除去最大的一路，剩下3路进行比较
    if (arr[p[i]] != a_max) {
      cmp[cmp_cnt++] = arr[p[i]];
    }
  }
  while (1) {                                                 //每次比较入列一个最小值
    compare(cmp[0], cmp[1], cmp[2], &a_max, &tmp[tmp_cnt++]); //比较选择的3路
    cmp_cnt = 0;
    for (int i = 0; i < 4; i++) {
      if (arr[p[i]] != a_max) {              //若第i路不是最大值
        if (arr[p[i]] == tmp[tmp_cnt - 1]) { //若第i路是最小值
          p[i]++;                            //第i路入列一个
        }
        cmp[cmp_cnt++] = arr[p[i]]; //第i路需要参与下次比较（因为不是最大值）
      }
      if (p[i] == r[i]) { // 若第i路已空
        int cl[3], cr[3];
        int count = 0;
        for (int j = 0; j < 4; j++) {
          if (j != i) {
            cl[count] = p[j];
            cr[count] = r[j];
            count++;
          }
        }
        merge3(arr, tmp, cl, cr, tmp_cnt); //退化为剩余三个序列的三路归并
        return;
      }
    }
  }
}

void merge_sort(int arr[], int tmp[], int step, int n)
/**********************************************************
归并排序
arr为原序列，tmp为归并排序所用临时序列
step为每个子序列长度
n为原序列总长度
***********************************************************/
{
  int i = 0;
  while (n - i >= 4 * step) { //长度足够4个子序列
    int l[4] = { i, i + step, i + 2 * step, i + 3 * step };
    int r[4] = { i + step, i + 2 * step, i + 3 * step, i + 4 * step };
    merge4(arr, tmp, l, r, i); //将４个有序子序列合并成一个4倍长度的子序列
    i = i + 4 * step;
  }
  if (n - i > 3 * step) { //长度不够4个子序列，超过3个
    int l[4] = { i, i + step, i + 2 * step, i + 3 * step };
    int r[4] = { i + step, i + 2 * step, i + 3 * step, n };
    merge4(arr, tmp, l, r, i); //将4个有序子序列合并成一个3倍长度的子序列
    return;
  }
  if (n - i > 2 * step) { //长度不大于3个子序列，超过2个
    int l[3] = { i, i + step, i + 2 * step };
    int r[3] = { i + step, i + 2 * step, n };
    merge3(arr, tmp, l, r, i); //将3个有序子序列合并成一个3倍长度的子序列
    return;
  }
  if (n - i > step) { //长度不大于2个子序列，超过1个
    int l[2] = { i, i + step };
    int r[2] = { i + step, n };
    merge2(arr, tmp, l, r, i); //将2个有序子序列合并成一个2倍长度的子序列
    return;
  }

  // 将剩下来的不大于一个序列长度的剩余元素并入
  for (int j = i; j < n; j++)
    tmp[j] = arr[j];
}

void sort(int n, int limit, int* a)
{
  int step = 1;
  while (step < n) {
    // 将a的元素按照step归并到temp
    merge_sort(a, temp, step, n);
    step <<= 2;
    // 将temp的元素按照step归并到a
    merge_sort(temp, a, step, n);
    step <<= 2;
  }
  return;
}
