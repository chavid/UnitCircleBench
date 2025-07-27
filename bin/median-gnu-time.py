#!/usr/bin/env python3

#!/usr/bin/env python3

import sys
import re
import statistics

filename = sys.argv[1]

checksum_re = re.compile('^\(checksum:\s+(.*)\)$')
gnu_time_re = re.compile('^\(GNU time:\s+real\s(.*)\ss,\s+user\s(.*)\ss,\s+sys\s(.*)\ss,\s(.*)\skBytes\)$')

checksum = False
real_times = []
user_times = []
sys_times = []
memories = []
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
  gnu_time_m = gnu_time_re.match(line.strip())
  if gnu_time_m:
    real_times.append(float(gnu_time_m.group(1)))
    user_times.append(float(gnu_time_m.group(2)))
    sys_times.append(float(gnu_time_m.group(3)))
    memories.append(int(gnu_time_m.group(4)))

print('(GNU median time: real {:0.3f} s, user {:0.3f} s, sys {:0.3f} s, {:5d} kBytes, checksum: {})'.format(
    round(statistics.median(real_times),3),
    round(statistics.median(user_times),3),
    round(statistics.median(sys_times),3),
    round(statistics.median(memories),3),
    checksum)
)
