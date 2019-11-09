def minDistance(word1, word2, num1, num2):
    if not word1:
        return len(word2 or '') or 0

    if not word2:
        return len(word1 or '') or 0

    size1 = num1
    size2 = num2

    last = 0
    tmp = list(range(size2 + 1))
    value = None

    for i in range(size1):
        tmp[0] = i + 1
        last = i
        # print word1[i], last, tmp
        for j in range(size2):
            if word1[i] == word2[j]:
                value = last
            else:
                value = 1 + min(tmp[j], tmp[j + 1])
                # print(last, tmp[j], tmp[j + 1], value)
            last = tmp[j + 1]
            tmp[j + 1] = value
        # print tmp
    return value


string = input()
string = list(filter(None, string.split(' ')))

num1 = int(string[0])
num2 = int(string[1])
k = int(string[2])

word1 = input()
word2 = input()
result = minDistance(word1, word2, num1, num2)

if result > k:
    print(-1)
else:
    print(result)
