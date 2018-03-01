import sys 

ifile = open(sys.argv[1])
ofile = open(sys.argv[2], 'w')

for line in ifile:
    numbers = list(map(int, line.split()))
    M, smth = numbers[0], numbers[1:]
    smth = list(map(lambda x: x - 1, smth))
    oline = [M] + smth
    ofile.write(' '.join(map(str, oline)) + '\n')

ofile.close()
ifile.close()