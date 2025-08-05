

#include "utilities.hh"
#include "unit-complex.hh"
#include "dyn-array.hh"
#include "complexes-aos.hh"
#include "complexes-soa.hh"

#include <iostream>
#include <cassert> // for assert
#include <valarray>
#include <vector>
#include <list>
#include <array>
#ifdef __GNUC__
#  ifndef __clang__
#    include <stdfloat>
#  endif
#endif
//#include <numbers>
//#include <cmath>


//================================================================
// Main
//================================================================

template< typename Collection >
void main_impl( std::size_t size, long long degree )
 {
  Collection collection(size) ;
  collection.pow(degree) ;
  auto res = collection.reduce() ;
  if (std::abs(1.-res.magnitude())<0.01)
   { std::cout<<myformat("(checksum: {})",res.argument())<<std::endl ; }
  else
   { std::cout<<myformat("(checksum: wrong magnitude {})",res.argument())<<std::endl ; }
 }

template< typename fp_t>
void main_aos( std::string & collection_name, std::size_t size, long long degree )
 {
  if (collection_name=="carray") main_impl<AoS<Complex<fp_t>*>>(size,degree) ;
  else if (collection_name=="dynarray") main_impl<AoS<DynArray<Complex<fp_t>>>>(size,degree) ;
  else if (collection_name=="array") main_impl<AoS<std::array<Complex<fp_t>,MAX_ARRAY_SIZE>>>(size,degree) ;
  else if (collection_name=="valarray") main_impl<AoS<std::valarray<Complex<fp_t>>>>(size,degree) ;
  else if (collection_name=="vector") main_impl<AoS<std::vector<Complex<fp_t>>>>(size,degree) ;
  else if (collection_name=="list") main_impl<AoS<std::list<Complex<fp_t>>>>(size,degree) ;
  else throw "unknown collection_name" ;
 }

template< typename fp_t>
void main_soa( std::string & collection_name, std::size_t size, long long degree )
 {
  if (collection_name=="carray") main_impl<Complexes<fp_t *>>(size,degree) ;
  else if (collection_name=="dynarray") main_impl<Complexes<DynArray<fp_t>>>(size,degree) ;
  else if (collection_name=="array") main_impl<Complexes<std::array<fp_t,MAX_ARRAY_SIZE>>>(size,degree) ;
  else if (collection_name=="valarray") main_impl<Complexes<std::valarray<fp_t>>>(size,degree) ;
  else if (collection_name=="vector") main_impl<Complexes<std::vector<fp_t>>>(size,degree) ;
  else if (collection_name=="list") main_impl<Complexes<std::list<fp_t>>>(size,degree) ;
  else throw "unknown collection_name" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==6) ;
  std::string arrangement_name(argv[1]) ;
  std::string collection_name(argv[2]) ;
  std::string fp_name(argv[3]) ;
  std::size_t size = {std::strtoull(argv[4],nullptr,10)} ;
  long long degree = {std::strtoll(argv[5],nullptr,10)} ;
  srand(1) ;
  
  if (arrangement_name=="aos")
   {
       if (fp_name=="float") time("main",main_aos<float>,collection_name,size,degree) ;
       else if (fp_name=="double") time("main",main_aos<double>,collection_name,size,degree) ;
       else if (fp_name=="long") time("main",main_aos<long double>,collection_name,size,degree) ;
#ifdef __GNUC__
#  ifndef __clang__
       else if (fp_name=="half") time("main",main_aos<std::float16_t>,collection_name,size,degree) ;
       else if (fp_name=="quad") time("main",main_aos<std::float128_t>,collection_name,size,degree) ;
#  endif
#endif
    else throw "unknown precision" ;
   }
  else if (arrangement_name=="soa")
   {
    if (fp_name=="float") time("main",main_soa<float>,collection_name,size,degree) ;
    else if (fp_name=="double") time("main",main_soa<double>,collection_name,size,degree) ;
    else if (fp_name=="long") time("main",main_soa<long double>,collection_name,size,degree) ;
#ifdef __GNUC__
#  ifndef __clang__
    else if (fp_name=="half") time("main",main_soa<std::float16_t>,collection_name,size,degree) ;
    else if (fp_name=="quad") time("main",main_soa<std::float128_t>,collection_name,size,degree) ;
#  endif
#endif
    else throw "unknown precision" ;
   }
  else throw "unknown arrangement_name" ;
 }
