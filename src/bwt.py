#!/usr/bin/env python3
# -*- coding: utf-8 -*-

# %%
def make_rank(tuples):
    c = 0
    result = [c] * len(tuples)
    for i in range(1, len(tuples)):
        if tuples[i][0] != tuples[i-1][0] or tuples[i][1] != tuples[i-1][1]:
            c += 1
        result[tuples[i][2]] = c
    return result


# %%
def make_SA(text):
    n = len(text)

    tuples = [(text[i], 0, i) for i in range(n)]
    tuples = sorted(tuples)

    text *= 2
    k = 0

    while 2**k < n:
        rank = make_rank(tuples)
        tuples = [(rank[i], rank[(i + 2**k) % n], i) for i in range(n)]
        tuples = sorted(tuples)
        k += 1

    SA = [x for (_, _, x) in tuples]
    return SA


# %%
def txt2bwt(text):
    SA = make_SA(text)
    return "".join([text[(x-1) % len(text)] for x in SA])


# %%
def get_alphabet(txt):
    return sorted(set(txt))


# %%
def construct_first(bwt):
    alphabet = get_alphabet(bwt)
    F = dict()
    for c in alphabet:
        F[c] = 0
    for c in bwt:
        F[c] += 1

    F_txt = ""
    for c in alphabet:
        F_txt += c * F[c]
    return F, F_txt


# %%
def construct_tally(bwt):
    alphabet = get_alphabet(bwt)
    tally = dict()
    for c in alphabet:
        tally[c] = [0]

    tally[bwt[0]][0] = 1
    for i in range(1, len(bwt)):
        for c in alphabet:
            if bwt[i] == c:
                tally[c].append(tally[c][-1] + 1)
            else:
                tally[c].append(tally[c][-1])

    return tally


# %%
def construct_prefix_sum(F_txt):
    # highest index of the characters lexicographically smaller than char in F
    result = dict()
    for i, c in enumerate(F_txt):
        if c not in result:
            result[c] = i - 1

    # result = {'$': -1, 'A': 0, 'B': 3}
    return result


# %%
def bwt2txt(bwt):
    F, F_txt = construct_first(bwt)
    F_jump = construct_prefix_sum(F_txt)
    L = bwt
    tally = construct_tally(bwt)

    txt = ""
    i = 0
    for _ in range(len(L)):
        txt += F_txt[i]
        i = F_jump[L[i]] + tally[L[i]][i]

    return txt


# %%
text = "aba_dfkh_bab$"

bwt = txt2bwt(text)
txt = bwt2txt(bwt)[::-1]

# %%
file = "SH.txt"
with open(file) as f:
    line = f.readline().replace(" ", "_") + "$"

bwt = txt2bwt(line)

# %%
with open("SH.bwt", "w") as f:
    f.write(bwt)
    f.flush()

# %%
n_runs = 0
prev = ""
for c in bwt:
    if prev != c:
        n_runs += 1
        prev = c

# 2288286 runs
