
Analysis of July 27 2025, on personnal machine.

## Fast check of list and -O0 vs -O2

Common configuration:
- Compiled with g++ c++23 -00 and -O2.
- Collections of 1024 doubles.
- Raised to their power of 10000.

| seconds  | O0 aos | O0 soa | O2 aos | O2 soa |
|----------|--------|--------|--------|--------|
| carray   | 0.105  | 0.089  | 0.008  | 0.009  |
| dynarray | 0.125  | 0.213  | 0.008  | 0.011  |
| array    | 0.121  | 0.188  | 0.009  | 0.011  |
| valarray | 0.123  | 0.208  | 0.008  | 0.010  |
| vector   | 0.125  | 0.207  | 0.009  | 0.011  |
| list     | 0.079  | 0.101  | 0.021  | 0.020  |

## More compute bound

Common configuration:
- Compiled with g++ c++23 -02 and -O3.
- Collections of 1024 doubles.
- Raised to their power of 100000.
- list is removed because too slow.

| seconds  | O2 aos | O2 soa | O3 aos | O3 soa |
|----------|--------|--------|--------|--------|
| carray   | 0.077  | 0.084  | 0.025  | 0.015  |
| dynarray | 0.077  | 0.085  | 0.025  | 0.016  |
| array    | 0.077  | 0.088  | 0.022  | 0.023  |
| valarray | 0.077  | 0.085  | 0.025  | 0.015  |
| vector   | 0.077  | 0.084  | 0.038  | 0.015  |

About the `array` flavor, looking the RAM declared by GNU time show that the program needs 1 Mo more RAM, due to the fact that our std::arrays are of size MAX_ARRAY_SIZE (65536) whatever the command line parameters.

Not understood yet:
- the bad performance of array with O3 soa ; because being on the stack ?
- the bad performance of vector with O3 aos ; a failed try to vectorize ?

## More I/O bound

Common configuration:
- Compiled with g++ c++23 -O3.
- Collections of 65536 doubles.
- Raised to their power of 31250.

| seconds  | aos   | soa   |
|----------|-------|-------|
| carray   | 0.891 | 0.726 |
| dynarray | 0.863 | 2.620 |
| array    | 0.836 | 0.777 |
| valarray | 0.866 | 0.700 |
| vector   | 1.463 | 0.730 |

Why those anomalies for `dynarray soa` and `vector aos` ?


## g++ c++23 O3 double 1024 100000

aos vector half   : pow  0.298 s, main  0.298 s, checksum: wrong magnitude -nan)
aos vector float  : pow  0.019 s, main  0.019 s, checksum: -1.1885905)
aos vector double : pow  0.038 s, main  0.038 s, checksum: -1.204002016812725)
aos vector long   : pow  0.391 s, main  0.392 s, checksum: -1.2040020167587235953)
aos vector quad   : pow 11.911 s, main 11.912 s, checksum: -1.2040020167587227184551944447154174)


aos carray   : pow 0.024 s, main 0.024 s, checksum: -1.204002016812725)
aos dynarray : pow 0.024 s, main 0.024 s, checksum: -1.204002016812725)
aos array    : pow 0.021 s, main 0.022 s, checksum: -1.204002016812725)
aos valarray : pow 0.024 s, main 0.024 s, checksum: -1.204002016812725)
aos vector   : pow 0.038 s, main 0.038 s, checksum: -1.204002016812725)
soa carray   : pow 0.014 s, main 0.015 s, checksum: -1.204002016812725)
soa dynarray : pow 0.025 s, main 0.025 s, checksum: -1.204002016812725)
soa array    : pow 0.034 s, main 0.035 s, checksum: -1.204002016812725)
soa valarray : pow 0.117 s, main 0.117 s, checksum: -1.2040020168119314)
soa vector   : pow 0.023 s, main 0.024 s, checksum: -1.204002016812725)
