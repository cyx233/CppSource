#encoding=utf-8

import random
import sys


def dataMaker(n, k):
    f = sys.stdout
    f.write(str(n) + " " + str(k) + "\n")
    list1 = [random.randrange(0, 2**64 - 1) for i in range(n)]
    for i in list1:
        b = '{:064b}'.format(i)
        f.write(b + "\n")


# 每组数据10个，长度为10
dataMaker(500000, 500000)
