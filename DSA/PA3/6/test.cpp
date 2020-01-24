#include "sort.h"
#include <cstdio>

inline int max(int a, int b) { return a > b ? a : b; }
inline int min(int a, int b) { return a < b ? a : b; }
int temp[1000010];

void merge2(int arr[], int tmp[], int l[], int r[], int target)
{
  int p[2] = { l[0], l[1] };
  int a_max;
  int tmp_cnt = target;
  while (1) {
    compare(arr[p[0]], arr[p[1]], arr[p[1]], &a_max, &tmp[tmp_cnt++]);
    if (a_max == arr[p[0]]) {
      p[1]++;
      if (p[1] == r[1]) {
        for (int i = p[0]; i < r[0]; i++)
          tmp[tmp_cnt++] = arr[i];
        return;
      }
    } else {
      p[0]++;
      if (p[0] == r[0]) {
        for (int i = p[1]; i < r[1]; i++)
          tmp[tmp_cnt++] = arr[i];
        return;
      }
    }
  }
}

void merge3(int arr[], int tmp[], int l[], int r[], int target)
{
  int p[3] = { l[0], l[1], l[2] };
  int a_max;
  int tmp_cnt = target;
  while (1) {
    compare(arr[p[0]], arr[p[1]], arr[p[2]], &a_max, &tmp[tmp_cnt++]);
    for (int i = 0; i < 3; i++) {
      if (arr[p[i]] == tmp[tmp_cnt - 1])
        p[i]++;
      if (p[i] == r[i]) {
        int cl[2], cr[2], count = 0;
        for (int j = 0; j < 3; j++) {
          if (j != i) {
            cl[count] = p[j];
            cr[count] = r[j];
            count++;
          }
        }
        merge2(arr, tmp, cl, cr, tmp_cnt);
        return;
      }
    }
  }
}

void merge4(int arr[], int tmp[], int l[], int r[], int target)
{
  int p[4] = { l[0], l[1], l[2], l[3] };
  int a_max, a_min;
  int tmp_cnt = target;
  int cmp_cnt = 0;
  int cmp[3] = { arr[p[0]], arr[p[1]], arr[p[2]] };
  compare(cmp[0], cmp[1], cmp[2], &a_max, &a_min);
  for (int i = 0; i < 4; i++) {
    if (arr[p[i]] != a_max) {
      cmp[cmp_cnt++] = arr[p[i]];
    }
  }
  while (1) {
    compare(cmp[0], cmp[1], cmp[2], &a_max, &tmp[tmp_cnt++]);
    cmp_cnt = 0;
    for (int i = 0; i < 4; i++) {
      if (arr[p[i]] != a_max) {
        if (arr[p[i]] == tmp[tmp_cnt - 1]) {
          p[i]++;
        }
        cmp[cmp_cnt++] = arr[p[i]];
      }
      if (p[i] == r[i]) {
        int cl[3], cr[3];
        int count = 0;
        for (int j = 0; j < 4; j++) {
          if (j != i) {
            cl[count] = p[j];
            cr[count] = r[j];
            count++;
          }
        }
        merge3(arr, tmp, cl, cr, tmp_cnt);
        return;
      }
    }
  }
}

void merge_sort(int arr[], int tmp[], int step, int n)
{
  int i = 0;
  while (n - i >= 4 * step) { // 将两两相邻的有序子序列合并成一个2倍长度的子序列
    int l[4] = { i, i + step, i + 2 * step, i + 3 * step };
    int r[4] = { i + step, i + 2 * step, i + 3 * step, i + 4 * step };
    merge4(arr, tmp, l, r, i);
    i = i + 4 * step;
  }
  if (n - i > 3 * step) { // 将两两相邻的有序子序列合并成一个2倍长度的子序列
    int l[4] = { i, i + step, i + 2 * step, i + 3 * step };
    int r[4] = { i + step, i + 2 * step, i + 3 * step, n };
    merge4(arr, tmp, l, r, i);
    return;
  }
  if (n - i > 2 * step) { // 将两两相邻的有序子序列合并成一个2倍长度的子序列
    int l[3] = { i, i + step, i + 2 * step };
    int r[3] = { i + step, i + 2 * step, n };
    merge3(arr, tmp, l, r, i);
    return;
  }
  if (n - i > step) { // 将两两相邻的有序子序列合并成一个2倍长度的子序列
    int l[2] = { i, i + step };
    int r[2] = { i + step, n };
    merge2(arr, tmp, l, r, i);
    return;
  }
  // 若剩余序列超过一个序列长度将再次合并不足两个序列长度的两个序列
  // 将剩下来的不大于一个序列长度的剩余元素并入
  for (int j = i; j < n; j++)
    tmp[j] = arr[j];
}

void sort(int n, int limit, int* a)
{
  int step = 1;
  while (step < n) {
    // 将arr的元素按照step归并到tmp
    merge_sort(a, temp, step, n);
    step <<= 2;
    // 将tmp的元素按照新step归并到arr
    merge_sort(temp, a, step, n);
    step <<= 2;
  }
  return;
}
