
#include "utilities.hh"
#include "unit-complex.hh"

#include <chrono>
#include <string_view>
#include <iostream>
#include <algorithm>


//================================================================
// SoA of complex numbers
//================================================================

void random_soa( auto && reals, auto && imags, std::size_t size )
 {
  for ( std::size_t i = 0 ; i < size ; ++i )
   { random_unit_complex(reals[i],imags[i]) ; }
 }

template< std::floating_point fp_t >
void ax_soa(
  fp_t * xreals, fp_t * ximags,
  fp_t const * areals, fp_t const * aimags, 
  std::size_t size, long long repeat )
 {
  for ( long long d = 0 ; d < repeat ; ++d )
   {
    for ( std::size_t i = 0 ; i<size ; ++i )
     {
      auto r_new = xreals[i]*areals[i] - ximags[i]*aimags[i] ;
      auto i_new = ximags[i]*areals[i] + xreals[i]*aimags[i] ;
      xreals[i] = r_new ;
      ximags[i] = i_new ;
     }
   }
 }

auto reduce_soa( auto && reals, auto && imags, std::size_t size )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  using fp_type = typename array_type::value_type ;
  Complex<fp_type> acc = make_unit_complex<fp_type>(1.,0.) ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   { acc *= Complex<fp_type>{reals[i],imags[i]} ; }
  return acc ;
 }

template< std::floating_point fp_t >
class Complexes1
 {
  public :  
    Complexes1( std::size_t size ) : m_rs(size), m_is(size)
     { random_soa(m_rs,m_is,m_rs.size()) ; }
    void pow( long long degree )
     {
      std::vector<fp_t> a_rs{m_rs}, a_is{m_is} ;
      ax_soa<fp_t>(m_rs.data(),m_is.data(),a_rs.data(),a_is.data(),m_rs.size(),degree-1) ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_rs.size()) ; }    
  private :
    std::vector<fp_t> m_rs, m_is ;
 } ;

template< std::floating_point fp_t >
class Complexes2
 {
  public :  
    Complexes2( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { random_soa(m_rs,m_is,size) ; }
    void pow( long long degree )
     {
      std::vector<fp_t> a_rs{m_rs}, a_is{m_is} ;
      ax_soa<fp_t>(m_rs.data(),m_is.data(),a_rs.data(),a_is.data(),m_size,degree-1) ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_size) ; }    
  private :
    std::size_t m_size ;
    std::vector<fp_t> m_rs, m_is ;
 } ;


#include <cassert> // for assert
#include <vector>


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
void main_soa( std::string & collection_tname, std::size_t size, long long degree )
 {
  if (collection_tname=="c1") main_impl<Complexes1<fp_t>>(size,degree) ;
  else if (collection_tname=="c2") main_impl<Complexes2<fp_t>>(size,degree) ;
  else throw "unknown collection_tname" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==5) ;
  std::string collection_tname(argv[1]) ;
  std::string fp_tname(argv[2]) ;
  std::size_t size = {std::strtoull(argv[3],nullptr,10)} ;
  long long degree = {std::strtoll(argv[4],nullptr,10)} ;
  srand(1) ;
  
  if (collection_tname=="c1")
   {
    if (fp_tname=="float") time("main",main_soa<float>,collection_tname,size,degree) ;
    else if (fp_tname=="double") time("main",main_soa<double>,collection_tname,size,degree) ;
    else throw "unknown precision" ;
   }
  else if (collection_tname=="c2")
   {
    if (fp_tname=="float") time("main",main_soa<float>,collection_tname,size,degree) ;
    else if (fp_tname=="double") time("main",main_soa<double>,collection_tname,size,degree) ;
    else throw "unknown precision" ;
   }
  else throw "unknown arrangement_tname" ;
 }
