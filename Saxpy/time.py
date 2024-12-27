#!/usr/bin/env python3

import os, sys
import re
import subprocess
import statistics

if (sys.argv[1][0:3]!='c++'):
  print('Command-line error: first argument should start with c++')
  exit(1)
CPP_LEVEL = sys.argv[1]

if (sys.argv[2][0]!='O'):
  print('Command-line error: second argument should start with O')
  exit(1)
OPT_LEVEL = sys.argv[2]

if (sys.argv[3][0]!='x'):
  print('Command-line error: third argument should start with x')
  exit(1)
NB_RUNS = int(sys.argv[3][1:])

FILE_RADIX = sys.argv[4]
RUN_ARGS = ' '.join(sys.argv[5:])

compile_cmd = "rm -f tmp.{radix}.exe && g++ -std={cpp} -{opt} -march=native -mtune=native -lpthread -ltbb {radix}.cpp -o tmp.{radix}.exe".format(
  radix=FILE_RADIX,cpp=CPP_LEVEL,opt=OPT_LEVEL)
run_cmd = "./tmp.{radix}.exe {args}".format(radix=FILE_RADIX,args=RUN_ARGS)

# Utility fonction

def run(cmd):
    proc = subprocess.run(
      cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True,
      executable='bash', universal_newlines=True, check=True
    )
    return proc.stdout.rstrip().split('\n')

# Compile & first run

expr_times = re.compile("^\((.*) time: (.*) us\)$")

times = {}

os.system(compile_cmd)
for line in run(run_cmd):
    match = expr_times.match(line)
    if expr_times.match(line):
        times[match.groups()[0]] = []
    else:
        print(line)

# Repeat timing

for irun in range(NB_RUNS):
    for line in run(run_cmd):
        match = expr_times.match(line)
        if expr_times.match(line):
            times[match.groups()[0]].append(int(match.groups()[1]))

# Display mean times

for ktime in times:
    #print("({} times: {} s)".format(ktime,times[ktime]))
    print("({} mean time: {} s)".format(ktime,round(statistics.mean(times[ktime])/1000000.,3)))

