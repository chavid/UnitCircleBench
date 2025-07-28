
# Analysis of July 28 2025, on ls-cassid.

## g++ c++23 O0 complexes

aos carray   float 1024 10000 (ram:  4732 kBytes, median times: real 0.130 s, user 0.130 s, sys 0.000 s, pow 0.134 s, main 0.134 s, checksum: 0.8155029)
aos dynarray float 1024 10000 (ram:  4716 kBytes, median times: real 0.130 s, user 0.130 s, sys 0.000 s, pow 0.134 s, main 0.135 s, checksum: 0.8155029)
aos array    float 1024 10000 (ram:  5692 kBytes, median times: real 0.130 s, user 0.130 s, sys 0.000 s, pow 0.135 s, main 0.136 s, checksum: 0.8155029)
aos valarray float 1024 10000 (ram:  4768 kBytes, median times: real 0.130 s, user 0.130 s, sys 0.000 s, pow 0.133 s, main 0.134 s, checksum: 0.8155029)
aos vector   float 1024 10000 (ram:  4700 kBytes, median times: real 0.130 s, user 0.130 s, sys 0.000 s, pow 0.134 s, main 0.135 s, checksum: 0.8155029)
aos list     float 1024 10000 (ram:  4752 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.125 s, main 0.125 s, checksum: 0.8155029)

soa carray   float 1024 10000 (ram:  4716 kBytes, median times: real 0.060 s, user 0.050 s, sys 0.000 s, pow 0.058 s, main 0.058 s, checksum: 0.8155029)
soa dynarray float 1024 10000 (ram:  4752 kBytes, median times: real 0.050 s, user 0.050 s, sys 0.000 s, pow 0.058 s, main 0.058 s, checksum: 0.8155029)
soa array    float 1024 10000 (ram:  5676 kBytes, median times: real 0.060 s, user 0.050 s, sys 0.000 s, pow 0.058 s, main 0.058 s, checksum: 0.8155029)
soa valarray float 1024 10000 (ram:  4716 kBytes, median times: real 0.470 s, user 0.470 s, sys 0.000 s, pow 0.477 s, main 0.478 s, checksum: 0.8155029)
soa vector   float 1024 10000 (ram:  4700 kBytes, median times: real 0.050 s, user 0.050 s, sys 0.000 s, pow 0.058 s, main 0.058 s, checksum: 0.8155029)
soa list     float 1024 10000 (ram:  4732 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.124 s, main 0.125 s, checksum: 0.8155029)

## g++ c++23 O2 complexes

aos carray   float 1024 10000 (ram:  4444 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.016 s, main 0.016 s, checksum: 0.81550205)
aos dynarray float 1024 10000 (ram:  4432 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.016 s, main 0.016 s, checksum: 0.81550205)
aos array    float 1024 10000 (ram:  5384 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.016 s, main 0.017 s, checksum: 0.81550205)
aos valarray float 1024 10000 (ram:  4424 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.017 s, main 0.017 s, checksum: 0.81550205)
aos vector   float 1024 10000 (ram:  4428 kBytes, median times: real 0.020 s, user 0.010 s, sys 0.000 s, pow 0.016 s, main 0.017 s, checksum: 0.81550205)
aos list     float 1024 10000 (ram:  4476 kBytes, median times: real 0.020 s, user 0.020 s, sys 0.000 s, pow 0.027 s, main 0.028 s, checksum: 0.81535614)

soa carray   float 1024 10000 (ram:  4456 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.012 s, main 0.012 s, checksum: 0.81535614)
soa dynarray float 1024 10000 (ram:  4464 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.012 s, main 0.012 s, checksum: 0.81535614)
soa array    float 1024 10000 (ram:  5396 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.016 s, main 0.017 s, checksum: 0.81535614)
soa valarray float 1024 10000 (ram:  4428 kBytes, median times: real 0.020 s, user 0.020 s, sys 0.000 s, pow 0.022 s, main 0.022 s, checksum: 0.8154577)
soa vector   float 1024 10000 (ram:  4456 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.012 s, main 0.012 s, checksum: 0.81535614)
soa list     float 1024 10000 (ram:  4456 kBytes, median times: real 0.020 s, user 0.020 s, sys 0.000 s, pow 0.027 s, main 0.028 s, checksum: 0.81535614)

## g++ c++23 O2 complexes

aos carray   float 1024 100000 (ram:  4440 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.123 s, main 0.123 s, checksum: -1.2693095)
aos dynarray float 1024 100000 (ram:  4432 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.123 s, main 0.124 s, checksum: -1.2693095)
aos array    float 1024 100000 (ram:  5416 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.123 s, main 0.123 s, checksum: -1.2693095)
aos valarray float 1024 100000 (ram:  4444 kBytes, median times: real 0.130 s, user 0.130 s, sys 0.000 s, pow 0.130 s, main 0.130 s, checksum: -1.2693095)
aos vector   float 1024 100000 (ram:  4428 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.123 s, main 0.123 s, checksum: -1.2693095)

soa carray   float 1024 100000 (ram:  4456 kBytes, median times: real 0.080 s, user 0.080 s, sys 0.000 s, pow 0.088 s, main 0.088 s, checksum: -1.2699027)
soa dynarray float 1024 100000 (ram:  4456 kBytes, median times: real 0.080 s, user 0.080 s, sys 0.000 s, pow 0.088 s, main 0.088 s, checksum: -1.2699027)
soa array    float 1024 100000 (ram:  5416 kBytes, median times: real 0.120 s, user 0.120 s, sys 0.000 s, pow 0.124 s, main 0.125 s, checksum: -1.2699027)
soa valarray float 1024 100000 (ram:  4436 kBytes, median times: real 0.170 s, user 0.170 s, sys 0.000 s, pow 0.172 s, main 0.172 s, checksum: -1.270004)
soa vector   float 1024 100000 (ram:  4436 kBytes, median times: real 0.080 s, user 0.080 s, sys 0.000 s, pow 0.088 s, main 0.089 s, checksum: -1.2699027)

## g++ c++23 O3 complexes

aos carray   float 1024 100000 (ram:  4532 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.017 s, main 0.017 s, checksum: -1.2699027)
aos dynarray float 1024 100000 (ram:  4532 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.017 s, main 0.017 s, checksum: -1.2699027)
aos array    float 1024 100000 (ram:  5448 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.015 s, main 0.016 s, checksum: -1.2699027)
aos valarray float 1024 100000 (ram:  4708 kBytes, median times: real 0.020 s, user 0.020 s, sys 0.000 s, pow 0.026 s, main 0.026 s, checksum: -1.2699027)
aos vector   float 1024 100000 (ram:  4460 kBytes, median times: real 0.020 s, user 0.020 s, sys 0.000 s, pow 0.026 s, main 0.026 s, checksum: -1.2699027)

soa carray   float 1024 100000 (ram:  4460 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.011 s, main 0.011 s, checksum: -1.2699027)
soa dynarray float 1024 100000 (ram:  4448 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.011 s, main 0.011 s, checksum: -1.2699027)
soa array    float 1024 100000 (ram:  5384 kBytes, median times: real 0.020 s, user 0.020 s, sys 0.000 s, pow 0.019 s, main 0.020 s, checksum: -1.2699027)
soa valarray float 1024 100000 (ram:  4432 kBytes, median times: real 0.050 s, user 0.050 s, sys 0.000 s, pow 0.057 s, main 0.057 s, checksum: -1.270004)
soa vector   float 1024 100000 (ram:  4416 kBytes, median times: real 0.010 s, user 0.010 s, sys 0.000 s, pow 0.011 s, main 0.011 s, checksum: -1.2699027)

## g++ c++23 O3 complexes

aos carray   float 65536 32768 (ram:  5048 kBytes, median times: real 0.430 s, user 0.420 s, sys 0.000 s, pow 0.422 s, main 0.430 s, checksum: wrong magnitude -0.94042915)
aos dynarray float 65536 32768 (ram:  5052 kBytes, median times: real 0.420 s, user 0.420 s, sys 0.000 s, pow 0.416 s, main 0.422 s, checksum: wrong magnitude -0.94042915)
aos array    float 65536 32768 (ram:  5476 kBytes, median times: real 0.420 s, user 0.420 s, sys 0.000 s, pow 0.419 s, main 0.425 s, checksum: wrong magnitude -0.94042915)
aos valarray float 65536 32768 (ram:  5064 kBytes, median times: real 0.770 s, user 0.770 s, sys 0.000 s, pow 0.767 s, main 0.775 s, checksum: wrong magnitude -0.94042915)
aos vector   float 65536 32768 (ram:  5040 kBytes, median times: real 0.730 s, user 0.730 s, sys 0.000 s, pow 0.726 s, main 0.734 s, checksum: wrong magnitude -0.94042915)

soa carray   float 65536 32768 (ram:  5044 kBytes, median times: real 0.360 s, user 0.360 s, sys 0.000 s, pow 0.360 s, main 0.368 s, checksum: wrong magnitude -0.94042915)
soa dynarray float 65536 32768 (ram:  5044 kBytes, median times: real 0.380 s, user 0.370 s, sys 0.000 s, pow 0.370 s, main 0.378 s, checksum: wrong magnitude -0.94042915)
soa array    float 65536 32768 (ram:  5432 kBytes, median times: real 0.700 s, user 0.690 s, sys 0.000 s, pow 0.695 s, main 0.701 s, checksum: wrong magnitude -0.94042915)
soa valarray float 65536 32768 (ram:  5348 kBytes, median times: real 3.130 s, user 3.130 s, sys 0.000 s, pow 3.131 s, main 3.137 s, checksum: wrong magnitude -0.9421375)
soa vector   float 65536 32768 (ram:  5020 kBytes, median times: real 0.360 s, user 0.360 s, sys 0.000 s, pow 0.359 s, main 0.367 s, checksum: wrong magnitude -0.94042915)


# Analysis of July 27 2025, on personnal machine.

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

