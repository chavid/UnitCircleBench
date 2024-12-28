
# UnitCircleBench

UNDER WORK : a set of old tests is under integration.

This is a C++ computational bench based on unit circle. The set of complexes of module 1 (unit circle) has this interesting property: if you multiply two such complexes, the result is still on the unit circle. This enables to build a bench that makes many, many multiplications with theoretically no risk of diverging to infinity or zero. Basically, we make a big array of unit complex numbers, them make the power of each of them (through multiplication), them reduce the array to a single number (through multiplication). Depending on the array size and the power degree, one can make the computation either cpu-bound or io-bound. Then we can compare the performances of different hardware, compilers, versions, optimization levels, kinds of array, SoA vs AoS, floating types.


## Requirements

A C++23 compiler is required, especially if you want to test with the new fixed width floating-point types.


## References

- [Unit Circle](https://en.wikipedia.org/wiki/Unit_circle)
- [Complex Numbers](https://en.wikipedia.org/wiki/Complex_number)
- [Fixed width floating-point types](https://en.cppreference.com/w/cpp/types/floating-point)


## Various observations

- When using raw C pointers, it may prove efficient to add some `__restrict__` keywords to the pointers, to tell the compiler that the memory areas pointed by the pointers do not overlap. This may help the compiler to vectorize the loops.
- It is important to systematically use `std::size_t` as the type for the indexes of the arrays. Let's say one use `unsigned` instead : the compiler will not dare to replace it with `std::size_t` because the developer may want to explicitly rely on the *wrap around* of `unsigned`. As a result, the compiler will need to constantly cast between the types, and will refuse to vectorize.
- About FMA : the files `fma.sh` and `fma.cpp` show that when one uses `-march=native`, this is trigerring the use of FMA instructions with `-O2`(and not with `-O0`). As a result, the numerical results are different between `-O2` and not with `-O0`. It seems that FMA is enabled by default and must be explicitly disabled with `-mno-fused-madd`.

