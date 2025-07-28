

#include "utilities.hh"
#include "unit-complex.hh"
#include "dyn-array.hh"
#include "complexes-aos.hh"
#include "complexes-soa.hh"

#include <iostream>
#include <format>
#include <cassert> // for assert
#include <valarray>
#include <vector>
#include <list>
#include <array>
#include <stdfloat>
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
   { std::cout<<std::format("(checksum: {})",res.argument())<<std::endl ; }
  else
   { std::cout<<std::format("(checksum: wrong magnitude {})",res.argument())<<std::endl ; }
 }

template< typename fp_t>
void main_aos( std::string & collection_tname, std::size_t size, long long degree )
 {
  if (collection_tname=="carray") main_impl<AoS<Complex<fp_t>*>>(size,degree) ;
  else if (collection_tname=="dynarray") main_impl<AoS<DynArray<Complex<fp_t>>>>(size,degree) ;
  else if (collection_tname=="array") main_impl<AoS<std::array<Complex<fp_t>,MAX_ARRAY_SIZE>>>(size,degree) ;
  else if (collection_tname=="valarray") main_impl<AoS<std::valarray<Complex<fp_t>>>>(size,degree) ;
  else if (collection_tname=="vector") main_impl<AoS<std::vector<Complex<fp_t>>>>(size,degree) ;
  else if (collection_tname=="list") main_impl<AoS<std::list<Complex<fp_t>>>>(size,degree) ;
  else throw "unknown collection_tname" ;
 }

template< typename fp_t>
void main_soa( std::string & collection_tname, std::size_t size, long long degree )
 {
  if (collection_tname=="carray") main_impl<Complexes<fp_t *>>(size,degree) ;
  else if (collection_tname=="dynarray") main_impl<Complexes<DynArray<fp_t>>>(size,degree) ;
  else if (collection_tname=="array") main_impl<Complexes<std::array<fp_t,MAX_ARRAY_SIZE>>>(size,degree) ;
  else if (collection_tname=="valarray") main_impl<Complexes<std::valarray<fp_t>>>(size,degree) ;
  else if (collection_tname=="vector") main_impl<Complexes<std::vector<fp_t>>>(size,degree) ;
  else if (collection_tname=="list") main_impl<Complexes<std::list<fp_t>>>(size,degree) ;
  else throw "unknown collection_tname" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==6) ;
  std::string arrangement_tname(argv[1]) ;
  std::string collection_tname(argv[2]) ;
  std::string fp_tname(argv[3]) ;
  std::size_t size = {std::strtoull(argv[4],nullptr,10)} ;
  long long degree = {std::strtoll(argv[5],nullptr,10)} ;
  srand(1) ;
  
  if (arrangement_tname=="aos")
   {
    if (fp_tname=="half") time("main",main_aos<std::float16_t>,collection_tname,size,degree) ;
    else if (fp_tname=="float") time("main",main_aos<float>,collection_tname,size,degree) ;
    else if (fp_tname=="double") time("main",main_aos<double>,collection_tname,size,degree) ;
    else if (fp_tname=="long") time("main",main_aos<long double>,collection_tname,size,degree) ;
    else if (fp_tname=="quad") time("main",main_aos<std::float128_t>,collection_tname,size,degree) ;
    else throw "unknown precision" ;
   }
  else if (arrangement_tname=="soa")
   {
    if (fp_tname=="half") time("main",main_soa<std::float16_t>,collection_tname,size,degree) ;
    else if (fp_tname=="float") time("main",main_soa<float>,collection_tname,size,degree) ;
    else if (fp_tname=="double") time("main",main_soa<double>,collection_tname,size,degree) ;
    else if (fp_tname=="long") time("main",main_soa<long double>,collection_tname,size,degree) ;
    else if (fp_tname=="quad") time("main",main_soa<std::float128_t>,collection_tname,size,degree) ;
    else throw "unknown precision" ;
   }
  else throw "unknown arrangement_tname" ;
 }
