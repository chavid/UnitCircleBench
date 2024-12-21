#!/usr/bin/env python3

import sys
filename = sys.argv[1]

import re
result_re = re.compile('^Result:\s+(.*)$')
gnu_time_re = re.compile('^GNU time:\s+(.*)\ss,\s(.*)\skBytes[.]$')

result = False
mean_time = 0.
mean_memory = 0.
nb_match = 0
file = open(filename,"r")
for line in file.readlines():
  result_m = result_re.match(line.strip())
  if result_m:
    if result:
      if result != result_m.group(1):
        print('Error: multiple results')
        sys.exit(1)
    else:
      result = result_m.group(1)  
  user_time_m = gnu_time_re.match(line.strip())
  if user_time_m:
    mean_time += float(user_time_m.group(1))
    mean_memory += float(user_time_m.group(2))
    nb_match += 1

print('{:0.2f} s, {:5d} kBytes, {}'.format(round(mean_time/nb_match,2),round(mean_memory/nb_match),result))
