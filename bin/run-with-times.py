#!/usr/bin/env python3

import os, sys
import re
import subprocess
import statistics

if (sys.argv[1][0]!='x'):
  print('Command-line error: third argument should start with x')
  exit(1)
NB_RUNS = int(sys.argv[1][1:])

FILE_RADIX = sys.argv[2]

RUN_ARGS = ' '.join(sys.argv[3:])

UNIT_BUILD = os.environ.get("UNIT_BUILD")
if (UNIT_BUILD==''):
  print('Environnement error: UNIT_BUILD undefined')
  exit(1)

run_cmd = "{time_prefix} {unit_build}/{radix}.exe {args}".format(
    time_prefix="\\time -f \"(GNU time: real %e s, user %U s, sys %S s, %M kBytes)\"",
    unit_build=UNIT_BUILD,radix=FILE_RADIX,args=RUN_ARGS)

# Utility fonction

def run(cmd):
    proc = subprocess.run(
      cmd, stdout=subprocess.PIPE, stderr=subprocess.STDOUT, shell=True,
      executable='bash', universal_newlines=True, check=True
    )
    return proc.stdout.rstrip().split('\n')

# Warm-up and checksum

expr_times = re.compile("^\((.*) time: (.*) us\)$")
expr_checksum = re.compile("^\(checksum: (.*)\)$")
expr_gnu_time = re.compile('^\(GNU time:\s+real\s(.*)\ss,\s+user\s(.*)\ss,\s+sys\s(.*)\ss,\s(.*)\skBytes\)$')

chrono_times = {}
checksum = False

for line in run(run_cmd):
    match_checksum = expr_checksum.match(line)
    match_times = expr_times.match(line)
    if expr_checksum.match(line):
        checksum = match_checksum.group(1)
    elif expr_times.match(line):
        chrono_times[match_times.group(1)] = []
    elif expr_gnu_time.match(line):
        gnu_times = { "real": [], "user": [], "sys": [] }
        gnu_rams = []
    else:
        print(line)

# Repeat timing

for irun in range(NB_RUNS):
    for line in run(run_cmd):
        match_checksum = expr_checksum.match(line)
        match_times = expr_times.match(line.strip())
        match_gnu_time = expr_gnu_time.match(line.strip())
        if expr_checksum.match(line):
            if match_checksum.group(1) != checksum:
              print('Error: multiple checksums')
              sys.exit(1)
        elif match_times:
            chrono_times[match_times.group(1)].append(int(match_times.group(2))/1000000.)
        elif match_gnu_time:
            gnu_times["real"].append(float(match_gnu_time.group(1)))
            gnu_times["user"].append(float(match_gnu_time.group(2)))
            gnu_times["sys"].append(float(match_gnu_time.group(3)))
            gnu_rams.append(int(match_gnu_time.group(4)))

# Display mean times and checksum

print("(ram: {:5d} kBytes, median times: ".format(statistics.median(gnu_rams)),end="")
for ktime in gnu_times:
    print("{} {:.3f} s, ".format(ktime,round(statistics.median(gnu_times[ktime]),3)),end="")
for ktime in chrono_times:
    print("{} {:.3f} s, ".format(ktime,round(statistics.median(chrono_times[ktime]),3)),end="")
print("checksum: {})".format(checksum))

