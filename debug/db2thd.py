#!/usr/bin/python3

import sys
from math import sqrt

mag_harmonic = 10 ** (float(input())/20)
sqaure_sum = 0

for i in range(4):
    db = float(input())
    sqaure_sum += 10 ** (db/10)

print(sqrt(sqaure_sum) / mag_harmonic)