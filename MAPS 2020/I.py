import collections
import itertools
import random
import subprocess

def powerset(iterable):
    "powerset([1,2,3]) --> () (1,) (2,) (3,) (1,2) (1,3) (2,3) (1,2,3)"
    s = list(iterable)
    return itertools.chain.from_iterable(itertools.combinations(s, r) for r in range(len(s) + 1))

def findCycles(G):
    reachable = [[False for i in G] for j in G]
    for i in range(len(G)):
        stack = [i]
        while stack:
            vertex = stack[-1]
            stack.pop()
            for j in G[vertex]:
                if not reachable[i][j]:
                    reachable[i][j] = True
                    stack.append(j)
    taken = [False] * len(G)
    cycles = []
    for vertices in itertools.chain.from_iterable(itertools.combinations(G.keys(), sz) for sz in range(len(G), 1, -1)):
        ok = True
        for i in range(len(vertices)):
            j = (i + 1) % len(vertices)
            ok = ok and reachable[vertices[i]][vertices[j]] and not taken[i]
        if ok:
            cycles.append(vertices)
            for i in range(len(vertices)):
                taken[vertices[i]] = True
    return reachable, tuple(cycles)


trials = 100
for _ in range(trials):
    N = random.randint(1, 10)
    M = random.randint(0, min(N * (N - 1) // 2, 2 * N))
    G = {i: [] for i in range(N)}
    count = 0
    while count < M:
        u = random.randint(0, N - 1)
        v = random.randint(0, N - 1)
        if (u == v) or (v in G[u]):
            continue
        else:
            G[u].append(v)
            count += 1

    testcase = f'{N} {M}\n'
    for u in sorted(G):
        for v in sorted(G[u]):
            testcase += f'{u} {v}\n'

    reachable, cycles = findCycles(G)
    roads = 0
    for i in range(N):
        for j in range(N):
            if i != j:
                if j not in G[i] and not reachable[j][i]:
                    reachable[i][j] = True
                    G[i].append(j)
                    for k in range(N):
                        reachable[i][k] = reachable[i][k] or reachable[j][k]
                    roads += 1

    answer = int(subprocess.run(["I.exe"], stdout=subprocess.PIPE, text=True, input=testcase).stdout)
    if roads != answer:
        print("Error:")
        print("    Expected =", roads)
        print("    Actual =", answer)
        print("    Test Case:")
        print(testcase)
        break