
# UnitCircleBench

UNDER WORK : a set of old tests is under integration.

This is a C++ computational bench based on unit circle. The set of complexes of module 1 (unit circle) has this interesting property: if you multiply two such complexes, the result is still on the unit circle. This enables to build a bench that makes many, many multiplications with theoretically no risk of diverging to infinity or zero. Basically, we make a big array of unit complex numbers, them make the power of each of them (through multiplication), them reduce the array to a single number (through multiplication). Depending on the array size and the power degree, one can make the computation either cpu-bound or io-bound. Then we can compare the performances of different hardware, compilers, versions, optimization levels, kinds of array, SoA vs AoS, floating types.


## User guide

Requirements
- Bash.
- GNU time.
- Python 3.
- Docker.

Run the complexes benchmark:
```
source env.bash
benchs/complexes.bash
```

## References

- [Unit Circle](https://en.wikipedia.org/wiki/Unit_circle)
- [Complex Numbers](https://en.wikipedia.org/wiki/Complex_number)
- [Fixed width floating-point types](https://en.cppreference.com/w/cpp/types/floating-point)


## Various observations

- When using raw C pointers, it may prove efficient to add some `__restrict__` keywords to the pointers, to tell the compiler that the memory areas pointed by the pointers do not overlap. This may help the compiler to vectorize the loops.
- It is important to systematically use `std::size_t` as the type for the indexes of the arrays. Let's say one use `unsigned` instead : the compiler will not dare to replace it with `std::size_t` because the developer may want to explicitly rely on the *wrap around* of `unsigned`. As a result, the compiler will need to constantly cast between the types, and will refuse to vectorize.
- GCC's default floating-point policy is to allow all the transformations that should improve the precision of the results. This includes
  - the evaluation of computational intermediates in 80-bit on old x86 CPUs (before the 64-bit era that made SSE mandatory).
  - the propagation of floating-point constants in extended precision,
  - the contraction of `a*b+c` into `fma(a, b, c)` (with `-O2`), corresponding to the default option `-ffp-contract=fast`. This can change the results, and is not always desirable. If one wants to keep the same results, one must add `-ffp-contract=off`. The files `fma.bash` and `fma.cpp` demonstrate this.
- Should not use `time()` from `utilities.hh` at low level, or this seems to sometimes prevent vectorization.
- There are many subtil interactions between inlining and vectorization, and a simple innocent choice, such as giving a size as an integer or as a call to `size()`, may hardly impact performance : see `subtil-size.bash`.
- GCC starts to inline at -O2 since release 12 (https://www.phoronix.com/forums/forum/software/programming-compilers/1283541-gcc-12-enables-auto-vectorization-for-o2-optimization-level).
- `__restrict__` did not help.


## References

- https://wiki.gentoo.org/wiki/GCC_optimization
- https://gcc.gnu.org/projects/tree-ssa/vectorization.html