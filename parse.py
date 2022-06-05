with open('dump.txt', 'r') as f:
    lines = [line.rstrip().split(']') for line in f.readlines()]
    srt = sorted(lines, key=lambda x:  int(x[1]))
    for item in srt:
        print(f"{item[0]} {item[1]} {item[2]}")
