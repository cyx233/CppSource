# encoding=utf-8

import random
import sys


def dataMaker(n, limit):
    f = sys.stdout
    f.write(str(n) + " " + str(limit) + "\n")
    nums = list(range(n))
    random.shuffle(nums)
    for i in nums:
        f.write(str(i)+" ")


# 每组数据10个，长度为10
dataMaker(1000000, 10300000)
