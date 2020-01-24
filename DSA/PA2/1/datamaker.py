#encoding=utf-8

import random
import sys


def dataMaker(n):
    f = sys.stdout
    f.write(str(n) + " 9\n")
    for i in range(1, n - 2):
        f.write("1 " + str(i + 1) + "\n")
    f.write("2 " + str(n - 1) + " " + str(n) + "\n")
    f.write("0\n")
    f.write("0\n")

    f.write("1\n")
    f.write("0\n")

    f.write("1\n")
    f.write("1 0\n")

    f.write("2\n")
    f.write("0\n")

    f.write("2\n")
    f.write("1 0\n")

    f.write("0\n")
    f.write(str(n - 2) + " ")
    for i in range(n - 3):
        f.write("0 ")
    f.write("1\n")
    f.write(str(n - 2) + " ")
    for i in range(n - 2):
        f.write("0 ")
    f.write("\n")
    f.write("0\n")

    f.write("1\n")
    f.write("0\n")

    f.write("1\n")
    f.write("1 0\n")

    f.write("2\n")
    f.write("0\n")

    f.write("2\n")
    f.write("1 0\n")


dataMaker(1000000)
