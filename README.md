# UnitCircleBench

This is a C++ computational bench based on unit circle. The set of complexes of module 1 (unit circle) has this property: if you multiply too such complexes, the result is still on the unit circle. This enables to build a bench that makes many, many multiplications with theoretically no risk of diverging to infinity. Basically, we make a big array of unit complex numbers, them make the power of each of them (through multiplication), them reduce the array to a single number (through multiplication). Depending on the array size and the power degree, one can make the computation either cpu-bound or io-bound. Then we can compare the performances of different hardware, compilers, versions, optimization levels, kinds of array, SoA vs AoS, floating types.

