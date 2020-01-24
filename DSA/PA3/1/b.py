a = input()

flag = 0

for i in range(1, 25):
    for j in range(2**i):
        b = ('{:0' + str(i) + 'b}').format(j)
        if a.find(b) == -1:
            print(b)
            flag = 1
            break
    if flag:
        break
