#!/usr/bin/env python3

import os, sys
import re
import subprocess
import statistics

CPP_COMPILER = sys.argv[1]
if (sys.argv[1]==''):
  print('Command-line error: lacking compiler')
  exit(1)

if (sys.argv[2][0:3]!='c++'):
  print('Command-line error: first argument should start with c++')
  exit(1)
CPP_LEVEL = sys.argv[2]

if (sys.argv[3][0]!='O'):
  print('Command-line error: second argument should start with O')
  exit(1)
OPT_LEVEL = sys.argv[3]

FILE_RADIX = sys.argv[4]

OTHER_ARGS = ' '.join(sys.argv[5:])
if (OTHER_ARGS!=''):
  print('Useless arguments: ',OTHER_ARGS)
  exit(1)

UNIT_SRC = os.environ.get("UNIT_SRC")
if (UNIT_SRC==''):
  print('Environnement error: UNIT_SRC undefined')
  exit(1)

UNIT_BUILD = os.environ.get("UNIT_BUILD")
if (UNIT_BUILD==''):
  print('Environnement error: UNIT_BUILD undefined')
  exit(1)

if CPP_COMPILER=="g++":
  CPP_OPTIONS = "-g -march=native -mtune=native -funroll-loops -Wall -Wextra -Wfatal-errors -lpthread -ltbb"
  compile_cmd = "rm -f {unit_build}/{radix}.exe && g++ -std={cpp} -{opt} {cpp_options} {unit_src}/{radix}.cpp -o {unit_build}/{radix}.exe".format(
    cpp_options=CPP_OPTIONS,unit_src=UNIT_SRC,unit_build=UNIT_BUILD,radix=FILE_RADIX,cpp=CPP_LEVEL,opt=OPT_LEVEL)
elif CPP_COMPILER=="clang++":
  CPP_OPTIONS = "-g -march=native -mtune=native -funroll-loops -Wall -Wextra -Wfatal-errors -stdlib=libc++ -lpthread -ltbb"
  compile_cmd = "rm -f {unit_build}/{radix}.exe && clang++ -std={cpp} -{opt} {cpp_options} {unit_src}/{radix}.cpp -o {unit_build}/{radix}.exe".format(
    cpp_options=CPP_OPTIONS,unit_src=UNIT_SRC,unit_build=UNIT_BUILD,radix=FILE_RADIX,cpp=CPP_LEVEL,opt=OPT_LEVEL)
else:
  print('Unknown compiler:',CPP_CPMPILER)
  exit(1)

# à considérer
# - `-mavx2` au lieu des natives
# - enlever le -funroll-loops ?
# - définir des "jeux d'options" que l'on peut facilement activer/desactiver par option au script.

if os.system(compile_cmd) != 0:
    print(f"COMPILATION FAILED with exit code: {result}")
    exit(1)

