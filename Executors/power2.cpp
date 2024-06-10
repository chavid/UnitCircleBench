#include <vector>

using Real = double ;
using Reals = std::vector<Real> ;

#include <cmath>

// random numbers in [1.-1./scal,1.]
void generate( Reals & rs, Real scale )
 {
  srand(1) ;
  for ( auto & r : rs )
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
  decltype(sz) i ;
  for ( i=0 ; i<sz ; ++i )
   { ys[i] = 1.0 ; }
  for ( int d=0 ; d<degree ; ++d )
   {
    for ( i=0 ; i<sz ; ++i )
     { ys[i] *= xs[i] ; }
   }
 }
void pow_impl_alg
 ( Reals const & xs, int degree, Reals & ys )
 {
  ys.assign(xs.size(),Real{1.}) ;
  for ( int d=0 ; d<degree ; ++d )
   {
    std::transform(xs.begin(),xs.end(),ys.begin(),ys.begin(),[]( Real x, Real y )
     { return x*y ; }) ;
   }
 }
template <class ExecutionPolicy>
void pow_impl
 ( Reals const & xs, int degree, Reals & ys, ExecutionPolicy policy )
 {
  ys.assign(xs.size(),Real{1.}) ;
  for ( int d=0 ; d<degree ; ++d )
   {
    std::transform(policy,xs.begin(),xs.end(),ys.begin(),ys.begin(),[]( Real x, Real y )
     { return x*y ; }) ;
   }
 }
void pow
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
 { std::cout<<"mean: "<<(std::accumulate(rs.begin(),rs.end(),Real{0.})/rs.size())<<"\n" ; }
 
#include <cassert>

// main program
int main ( int argc, char * argv[] )
 {
  assert(argc==4) ;
  std::size_t dim {std::stoul(argv[1])} ;
  int degree = atoi(argv[2]) ;
  std::string policy(argv[3]) ;

  // prepare input
  Reals input(dim) ;
  generate(input,degree) ;
   
  // compute ouput
  Reals output(dim) ;
  pow(input,degree,output,policy) ;
  
  // post-process
  postprocess(output) ;
 }