
#include "utilities.hh"

#include <vector>
#include <array>

std::size_t const  BLOCK {4} ;
using Real = double ;
using RealBlock = std::array<Real, BLOCK> ;
using Reals = std::vector<RealBlock> ;

#include <cmath>

// random numbers in [1.-1./scal,1.]
void generate( Reals & rs, Real scale )
 {
  srand(1) ;
  for ( auto & rb : rs )
  for ( auto & r : rb )
   { r = 1.-rand()/scale/RAND_MAX ; }
 }

#include <string_view>
#include <algorithm>
#include <execution>

using namespace std::literals ;

// compute xs^degree and store it into ys
void pow_impl_raw
 ( Reals const & xs, int degree, Reals & ys )
 {
  auto sz {xs.size()} ;
  for ( decltype(sz) i=0 ; i<sz ; ++i )
   {
    ys[i].fill(1.) ;
    for ( int d=0 ; d<degree ; ++d )
     {
      auto j { BLOCK} ;
      for ( j=0 ; j< BLOCK ; ++j )
       { ys[i][j] *= xs[i][j] ; }
     }
   }
 }
void pow_impl_alg
 ( Reals const & xs, int degree, Reals & ys )
 {
  std::transform(xs.begin(),xs.end(),ys.begin(),[degree](  RealBlock const & xb )
   {
    RealBlock yb ;
    yb.fill(1.) ;
    for ( int d=0 ; d<degree ; ++d )
     {
      auto j {BLOCK} ;
      for ( j=0 ; j< BLOCK ; ++j )
       { yb[j] *= xb[j] ; }
     }
    return yb ;
   }) ;
 }
template <class ExecutionPolicy>
void pow_impl
 ( Reals const & xs, int degree, Reals & ys, ExecutionPolicy policy )
 {
  std::transform(policy,xs.begin(),xs.end(),ys.begin(),[degree](  RealBlock const & xb )
   {
    RealBlock yb ;
    yb.fill(1.) ;
    for ( int d=0 ; d<degree ; ++d )
     {
      auto j {BLOCK} ;
      for ( j=0 ; j< BLOCK ; ++j )
       { yb[j] *= xb[j] ; }
     }
    return yb ;
   }) ;
 }
void my_pow
 ( Reals const & xs, int degree, Reals & ys, std::string_view policy )
 {
  if (policy=="raw"sv)
   { pow_impl_raw(xs,degree,ys) ; }
  else if (policy=="alg"sv)
   { pow_impl_alg(xs,degree,ys) ; }
  else if (policy=="seq"sv)
   { pow_impl(xs,degree,ys,std::execution::seq) ; }
  else if (policy=="par"sv)
   { pow_impl(xs,degree,ys,std::execution::par) ; }
  else if (policy=="unseq"sv)
   { pow_impl(xs,degree,ys,std::execution::unseq) ; }
  else if (policy=="par_unseq"sv)
   { pow_impl(xs,degree,ys,std::execution::par_unseq) ; }
  else
   { throw "unknown policy" ; }
 }

#include <numeric>
#include <iostream>

// compute the mean 
void postprocess( Reals const & rs )
 {
  Real sum {0.} ;
  for ( auto & rb : rs )
  for ( auto & r : rb )
   { sum += r  ; }
  std::cout<<"(checksum: "<<(sum/(rs.size()*BLOCK))<<")\n" ;
 }

#include <cassert>

// main program
int main ( int argc, char * argv[] )
 {
  assert(argc==4) ;
  std::size_t dim {std::stoul(argv[1])} ;
  assert(dim% BLOCK==0) ;
  int degree = atoi(argv[2]) ;
  std::string policy(argv[3]) ;

  // prepare input
  Reals input(dim/BLOCK) ;
  generate(input,degree) ;
   
  // compute ouput
  Reals output(dim/BLOCK) ;
  time("pow",my_pow,input,degree,output,policy) ;
  
  // post-process
  postprocess(output) ;
 }