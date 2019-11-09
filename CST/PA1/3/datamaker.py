import random
import sys


def dataMaker(n1, n2, k):
    f = sys.stdout
    f.write(str(n1) + " " + str(n2) + " " + str(k) + "\n")
    S = ""
    for i in range(n1 + n2):
        S += str(random.randint(0, 9))

    S1 = S[:n1]
    S2 = S[n1:]
    f.write(S1 + '\n' + S2)


dataMaker(5000, 500, 50000)
