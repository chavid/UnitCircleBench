
#include "utilities.hh"
#include "unit-complex.hh"
#include "dyn-array.hh"

#include <iostream>
#include <format>
#include <cassert> // for assert
#include <vector>


//================================================================
// AoS
//================================================================

void init_random_aos( auto && collection )
 {
  using collection_type = std::remove_reference_t<decltype(collection)> ;
  using value_type = typename collection_type::value_type ;
  using fp_type = typename value_type::fp_type ;
  for ( auto & elem : collection )
   { elem = make_random_unit_complex<fp_type>() ; }
 }

template< std::floating_point fp_t >
void ax_aos( Complex<fp_t> const * a, Complex<fp_t> * x, std::size_t size, long long repeat )
 {
  for ( long long d = 0 ; d<repeat ; ++d )
   {
    for ( std::size_t i = 0 ; i<size ; ++i )
     {
      x[i] *= a[i] ;
     }
   }
 }

auto reduce_aos( auto && collection )
 {
  using collection_type = std::remove_reference_t<decltype(collection)> ;
  using value_type = typename collection_type::value_type ;
  using fp_type = typename value_type::fp_type ;
  value_type res  = make_unit_complex<fp_type>(1.,0.) ;
  for ( auto const & elem : collection )
   { res *= elem ; }
  return res ;
 }

template< std::floating_point fp_t >
class ComplexesAoS : public std::vector<Complex<fp_t>>
 {
  public :
    ComplexesAoS( std::size_t a_size ) : std::vector<Complex<fp_t>>(a_size)
     { init_random_aos(*this) ; }
    void pow( long long degree )
     {
      std::vector<Complex<fp_t>> a{*this} ;
      ax_aos(a.data(),this->data(),this->size(),degree-1) ;
     }
    Complex<fp_t> reduce() const
     { return reduce_aos(*this) ; }
 } ;


//================================================================
// SoA of complex numbers
//================================================================

void init_random_soa( auto && reals, auto && imags, std::size_t size )
 {
  for ( std::size_t i = 0 ; i < size ; ++i )
   { random_unit_complex(reals[i],imags[i]) ; }
 }

template< std::floating_point fp_t >
void ax_soa
 ( fp_t const * areals, fp_t const * aimags,
   fp_t * xreals, fp_t * ximags,
   std::size_t size, long long repeat )
 {
  for ( long long d = 0 ; d <repeat ; ++d )
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
class ComplexesSoA
 {
  public :  
    ComplexesSoA( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { init_random_soa(m_rs,m_is,m_size) ; }
    void pow( long long degree )
     {
      auto a_rs{m_rs}, a_is{m_is} ;
      ax_soa<fp_t>(a_rs.data(),a_is.data(),m_rs.data(),m_is.data(),m_size,degree-1) ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_size) ; }    
  private :
    std::size_t m_size ;
    std::vector<fp_t> m_rs, m_is ;
 } ;


//================================================================
// Main
//================================================================

template< typename Collection >
void main_impl( std::size_t size, long long degree )
 {
  Collection collection(size) ;
  collection.pow(degree) ;
  auto res = collection.reduce() ;
  std::cout<<std::format("(checksum: {} {})",res.magnitude(),res.argument())<<std::endl ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==5) ;
  std::string arrangement_tname(argv[1]) ;
  std::string fp_tname(argv[2]) ;
  std::size_t size = {std::strtoull(argv[3],nullptr,10)} ;
  long long degree = {std::strtoll(argv[4],nullptr,10)} ;
  srand(1) ;
  
  if (arrangement_tname=="aos")
   {
    if (fp_tname=="float") time("main",main_impl<ComplexesAoS<float>>,size,degree) ;
    else if (fp_tname=="double") time("main",main_impl<ComplexesAoS<double>>,size,degree) ;
    else throw "unknown precision" ;
   }
  else if (arrangement_tname=="soa")
   {
    if (fp_tname=="float") time("main",main_impl<ComplexesSoA<float>>,size,degree) ;
    else if (fp_tname=="double") time("main",main_impl<ComplexesSoA<double>>,size,degree) ;
    else throw "unknown precision" ;
   }
  else throw "unknown arrangement_tname" ;
 }
