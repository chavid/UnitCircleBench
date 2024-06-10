#!/usr/bin/env python3

import sys
filename = sys.argv[1]

import re
check_result_re = re.compile('^mean:\s+(.*)$')
real_time_re = re.compile('^real\s+([0-9\.]+)m([0-9\.]+)s$')
user_time_re = re.compile('^user\s+([0-9\.]+)m([0-9\.]+)s$')
sys_time_re = re.compile('^sys\s+([0-9\.]+)m([0-9\.]+)s$')

def compute_time(match):
  minutes = float(match.group(1))
  seconds = float(match.group(2))
  return 60*minutes+seconds

check_result = "0.627051"
mean_time = 0.
mean_real_time = 0.
mean_user_time = 0.
mean_sys_time = 0.
nb_real_time = 0
nb_user_time = 0
nb_sys_time = 0

file = open(filename,"r")
for line in file.readlines():
  mres = check_result_re.match(line.strip())
  if mres:
    if (mres.group(1)!=check_result):
      print('{} FAILING with {}'.format(sys.argv[1],mres.group(1)))
      sys.exit(1)
  mrt = real_time_re.match(line.strip())
  if mrt:
      mean_real_time += compute_time(mrt)
      nb_real_time += 1
  mut = user_time_re.match(line.strip())
  if mut:
      mean_user_time += compute_time(mut)
      nb_user_time += 1
  mst = sys_time_re.match(line.strip())
  if mst:
      mean_sys_time += compute_time(mst)
      nb_sys_time += 1

print('{:20} {:.2f} s {:.2f} s {:.2f} s'.format(sys.argv[3],
  mean_real_time/nb_real_time,
  mean_user_time/nb_user_time,
  mean_sys_time/nb_sys_time))
