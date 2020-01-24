#encoding=utf-8

import random
import sys


def dataMaker(n):
    f = sys.stdout
    f.write(str(n) + "\n")

    templ = 2
    tempr = 2
    for i in range(1, n):
        templ = random.randint(templ, n)
        tempr = max(tempr, templ)
        tempr = random.randint(tempr, n)
        tempt = random.randint(1, 1000000000)
        f.write(str(templ) + " " + str(tempr) + " " + str(tempt) + "\n")


dataMaker(5)
