# encoding=utf-8

import random
import sys


def dataMaker(n, m, a, b):
    f = sys.stdout
    random.seed()
    f.write(str(n) + "\n")
    S = set()
    while len(S) < n:
        S.add(random.randint(a, b))
    for i in S:
        f.write(str(i) + ' ')
    f.write('\n')
    while len(S) < n:
        S.add(random.randint(a, b))
    for i in S:
        f.write(str(i) + ' ')
    f.write('\n')
    f.write(str(m) + '\n')
    S = set()
    while len(S) < m:
        S.add((random.randint(a, b), random.randint(a, b)))
    for i in S:
        f.write(str(i[0]) + " " + str(i[1]) + "\n")


# n for points, m for questions
dataMaker(20000, 200, 1, 2**31)
