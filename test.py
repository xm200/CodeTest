a = 3
b = a + 3
c = int(input())
print(c)


if c == "a":
    if a == 3:
        print(c)
    elif a == 2:
        if c == 3:
            print(b)
        else:
            print(a)
    else:
        print(a)
else:
    if a == 8:
        print("b")
    else:
        print("aboba")