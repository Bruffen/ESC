#!/usr/bin/env python3
#Script that formats data from the output of mpstat
#and generates files for LaTeX plots

import fileinput
import os
import glob
import re
import sys
import getopt
import subprocess
import math

def main():
    args, other = getopt.getopt(sys.argv[1:], 's')
    dictargs = dict(args)

    text = ""
    filename = other[len(other) - 1]
    for line in fileinput.input(filename):
        text += line

    avg = 0
    count = 0
    root = "cpu_usage_plots2/"
    #dirName = re.search(r"_([A-Z]{2}_gccO[0-9]+)\.txt", filename)[1]
    dirName = re.search(r"_([A-Z]{2}_omp[0-9]+)\.txt", filename)[1]
    if not os.path.exists(root + dirName):
        os.mkdir(root + dirName)
    
    for cpu in range(0, 32):
        regex = r"[0-9]{2}:[0-9]{2}:[0-9]{2}\s+" + f"{cpu}" + r"\s+(.*?) "
        values = re.findall(regex, text)

        f = open(root + dirName + f"/cpu{cpu}.txt", "w")

        for i in range(0, len(values)):                
            avg += float(values[i])
            count += 1
            f.write(f"{i} {values[i]}\n")
        f.close()

    avg = avg / count
    a = open(root + dirName + "/avg.txt", "w")
    a.write(f"Average: {avg}")
    a.close()

main()
