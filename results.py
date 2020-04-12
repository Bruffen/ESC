#!/usr/bin/env python3

import fileinput
import os
import glob
import re
import sys
import getopt
import subprocess
import math

def average(results):
    average = 0.0
    for r in results:
        average += r

    return average / len(results)

def average_kbest(results, k):
    average = 0.0
    for i in range(0, k):
        average += results[i]

    return average / k

def median(results):
    median = 0.0
    length = len(results)
    if length % 2 == 0:
        median = (results[int(length / 2)] + results[int(length / 2 + 1)]) / 2
    else:
        median = results[math.ceil(length / 2)]  
    return median

def calculateResults(res, multiplier, unit):
    avg = average(res) * multiplier
    k = 5
    avg_k = average_kbest(res, k) * multiplier
    mdian = median(res) * multiplier
    print(f"Average: \t\t{avg} {unit}\nAverage of {k} best: \t{avg_k} {unit}\nMedian: \t\t{mdian} {unit}\n")

def main():
    args, other = getopt.getopt(sys.argv[1:], 'pst')
    dictargs = dict(args)

    regex = r".*\nTotal: ([0-9]+.[0-9]*).*\nTime: ([0-9]+.[0-9]*)"

    path = other[0]
    for filename in glob.glob(os.path.join(path, '*.txt')):
        text = ""
        print(f"\n{filename}")
        for line in fileinput.input(filename):
            text += line

        times = []
        total = []

        print("aa")
        times = re.findall(r"Time in seconds =\s+([0-9]+.[0-9]*)\n", text)
        
        for t in times:
            total.append(float(t))

        total = sorted(total)
        print(total)
        calculateResults(total, 1, "seconds")


        

main()