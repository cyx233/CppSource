#encoding=utf-8

import random
import sys


def dataMaker(n):
    f = sys.stdout
    list1 = [random.randrange(0, 2) for i in range(n)]
    for i in list1:
        f.write(str(i))


dataMaker(2**24)
