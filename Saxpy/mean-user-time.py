#!/usr/bin/env python3

import sys
filename = sys.argv[1]

import re
user_time_re = re.compile('^user\s+0m(.*)s$')

mean_time = 0.
nb_match = 0
file = open(filename,"r")
for line in file.readlines():
  m = user_time_re.match(line.strip())
  if m:
      mean_time += float(m.group(1))
      nb_match += 1

print('{} {:0.2} s'.format(sys.argv[2:],mean_time/nb_match))
