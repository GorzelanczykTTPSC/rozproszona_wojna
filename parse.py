import re 
ships = 6
max_clock = 64

logarr = [[] for _ in range(max_clock)]

pattern = re.compile(r"^\[id: ([0-9]+)\] \[clock: ([0-9]+)\]")

with open('dump4.txt', 'r') as f:
    lines = [line for line in f.readlines()]
    for line in lines:
        result = pattern.match(line)
        #print(f"ID: {result.group(1)} Clock: {result.group(2)}")
        logarr[int(result.group(2))].append(line)

def compareLine(a):
    return int(pattern.match(a).group(1))


for idx, subarr in enumerate(logarr):
    logarr[idx]  = sorted(subarr, key=compareLine)

with open('dump_result4.txt', 'w') as f:
    for sub in logarr:
        for line in sub:
            f.write(line)