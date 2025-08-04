
#include <kwk/container.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/context/sycl/context.hpp>

#include <iostream>
#include <cassert> // for assert
#include <vector>
#include <algorithm>
#include <cstdlib>  // rand
#include <cmath>    // cos, sin, sqrt, atan

struct complex
 { double r, i ; } ;

int main( int argc, char * argv[] )
 {
  assert(argc==3) ;
  std::size_t size = {std::strtoull(argv[1],nullptr,10)} ;
  long long degree = {std::strtoll(argv[2],nullptr,10)} ;
  srand(1) ;

  std::vector<complex> xs(size) ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   {
    auto e = std::rand() ;
    xs[i].r = std::cos(e) ;
    xs[i].i = std::sin(e) ;
   }

  std::vector<complex> as{xs} ;
  auto av = kwk::view{ kwk::source = as.data(), kwk::of_size(size) } ;
  auto xv = kwk::view{ kwk::source = xs.data(), kwk::of_size(size) } ;
  kwk::sycl::context context{::sycl::gpu_selector_v} ;
  kwk::transform(context,[](auto a, auto x)
   {
    for ( long long d = 0 ; d <(degree-1) ; ++d )
     {
      auto tmp = x.r*a.r - x.i*a.i ;
      x.i = x.i*a.r + x.r*a.i ;
      x.r = tmp ;
     }
    return x ;
   },xv,av,xv) ;
  std::cout<<std::endl ;
 }
