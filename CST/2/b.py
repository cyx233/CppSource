n = int(input())

x_string = input()
b_string = input()
x = x_string.split(' ')
x = list(filter(None, x))
intx = []
for i in x:
    intx.append(int(i))
x = intx
b = b_string.split(' ')
b = list(filter(None, b))

intb = []
for i in b:
    intb.append(int(i))
b = intb

x.sort()
b.sort()

k = []
for i in range(0, n):
    tempx = int(x[i])
    tempb = int(b[i])
    k.append(-float(tempx) / float(tempb))

num = int(input())
for i in range(0, num):
    qustion = input()
    mx = float(qustion.split(' ')[0])
    my = float(qustion.split(' ')[1])

    temp = mx * k[n - 1] + float(b[n - 1])
    if (temp <= my):
        print(n)
        continue
    temp = mx * k[0] + float(b[0])
    if (temp > my):
        print(0)
        continue

    for i in range(0, num - 1):
        temp = mx * k[i] + float(b[i])
        temp2 = mx * k[i + 1] + float(b[i + 1])
        if temp <= my and temp2 > my:
            print(i + 1)
            break
