import random 

n = 100
qs = 10
print(n, qs)
a = [i for i in range(n)]
random.shuffle(a)
for x in a:
    print(x, end=" ")
print()

for i in range(qs):
    q = sorted(list([random.randint(0,n), random.randint(0,n)]))
    print(q[0], q[1])

