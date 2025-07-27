#!/usr/bin/env python3

import sys
import re
import statistics

filename = sys.argv[1]

checksum_re = re.compile('^\(checksum:\s+(.*)\)$')
real_time_re = re.compile('^real\s+([0-9\.]+)m([0-9\.]+)s$')
user_time_re = re.compile('^user\s+([0-9\.]+)m([0-9\.]+)s$')
sys_time_re = re.compile('^sys\s+([0-9\.]+)m([0-9\.]+)s$')

def compute_time(match):
  minutes = float(match.group(1))
  seconds = float(match.group(2))
  return 60*minutes+seconds

checksum = False
real_times = []
user_times = []
sys_times = []

file = open(filename,"r")
for line in file.readlines():
  checksum_m = checksum_re.match(line.strip())
  if checksum_m:
    if checksum:
      if checksum != checksum_m.group(1):
        print('Error: multiple checksums')
        sys.exit(1)
    else:
      checksum = checksum_m.group(1)  
  mrt = real_time_re.match(line.strip())
  if mrt:
      real_times.append(float(compute_time(mrt)))
  mut = user_time_re.match(line.strip())
  if mut:
      user_times.append(float(compute_time(mut)))
  mst = sys_time_re.match(line.strip())
  if mst:
      sys_times.append(float(compute_time(mst)))

print('(Bash median time: real {:0.3f} s, user {:0.3f} s, sys {:0.3f} s, checksum: {})'.format(
    round(statistics.median(real_times),3),
    round(statistics.median(user_times),3),
    round(statistics.median(sys_times),3),
    checksum)
)

