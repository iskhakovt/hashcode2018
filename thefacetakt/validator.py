iflie = open(input())
ofile = open(input())

R, C, F, N, B, T = map(int, ifile.readline().split())

rides = []

for i in range(N):
    rides.append(list(map(int, ifile.readline().split())))

out = ofile.readlines()

while (out and not out[-1].strip()):
    out.pop()

assert(len(out) == F):

free_rides = set(range(N))

money = 0

for line in out:
    numbers = list(map(int, line.split()))
    M, smth = numbers[0], numbers[1:]
    assert(M == len(smth))

    x, y = (0, 0)
    time = 0

    for dest in smth:
        dest -= 1
        assert(dest in free_rides)
        free_rides.remove(dest)
        sx, sy, fx, fy, s, f = rides[dest]
        rdist = abs(sx - x) + abs(sy - y)
        rtime = abs(fx - sx) + abs(fy - sy)
        # assert(max(time + rdist, s) + rtime <= f)
        if (time + rdist <= s):
            money += B
        time = max(time + rdist, s) + rtime
        if (time <= T and time <= f):
            money += rdist
        x, y = (fx, fy)

print(money)