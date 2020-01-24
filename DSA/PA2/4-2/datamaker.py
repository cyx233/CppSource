#encoding=utf-8

import random
import sys


def dataMaker(n, m):
    f = sys.stdout
    f.write(str(n) + ' ' + str(m) + "\n")
    list1 = [random.randrange(1, n + 1) for i in range(2 * m)]
    for i in range(m):
        S = ""
        if list1[i] % 3:
            S += 'Q '
        else:
            S += 'H '
        if list1[i] > list1[i + m]:
            S += str(list1[i + m]) + " " + str(list1[i])
        else:
            S += str(list1[i]) + " " + str(list1[i + m])
        f.write(S + "\n")


dataMaker(2**31 - 1, 200000)
