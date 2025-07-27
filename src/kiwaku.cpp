
#include <iostream>
#include <cassert> // for assert
#include <cstdlib> // for rand
#include <valarray>
#include <vector>
#include <list>
#include <array>
#include <stdfloat>
#include <numbers>
#include <cmath>
#include <format>

#include "utilities.hh"


//================================================================
// Unit complexes
//================================================================

void cast_and_store( std::floating_point auto & var, std::floating_point auto val  )
 {
  using fp_type = std::remove_reference_t<decltype(var)> ;
  var = static_cast<fp_type>(val) ;
 }

void random_unit_complex( std::floating_point auto & r, std::floating_point auto & i  )
 {
  auto a = std::rand() ;
  cast_and_store(r,std::cos(a)) ;
  cast_and_store(i,std::sin(a)) ;
 }

template< std::floating_point fp_t >
class Complex {

  public :

    using fp_type = fp_t ;

    Complex( fp_t a_r, fp_t a_i ) : m_r{a_r}, m_i{a_i} {}
    Complex() = default ;

    fp_t real() const { return m_r ; }
    fp_t imag() const { return m_i ; }
    fp_t magnitude() const { return std::sqrt(m_r*m_r+m_i*m_i) ; }
    fp_t argument() const { return std::atan(m_i/m_r) ; }

    Complex & operator*=( Complex const & other ) {
      auto r = m_r*other.m_r - m_i*other.m_i ;
      auto i = other.m_r*m_i + m_r*other.m_i ;
      m_r = r ;
      m_i = i ;
      return *this ;
    }
    friend Complex operator*( Complex const & lhs, Complex const & rhs ) {
      auto r = lhs.m_r*rhs.m_r - lhs.m_i*rhs.m_i ;
      auto i = rhs.m_r*lhs.m_i + lhs.m_r*rhs.m_i ;
      return Complex{r,i} ;
    }

  private :

    fp_t m_r {}, m_i {} ;

 } ;

template< std::floating_point fp_t1, std::floating_point fp_t2 >
Complex<fp_t1> make_unit_complex( fp_t2 a_r, fp_t2 a_i )
 {
  fp_t1 r, i ;
  cast_and_store(r,a_r) ;
  cast_and_store(i,a_i) ;
  return Complex<fp_t1>{r,i} ;
 }

template< std::floating_point fp_t >
Complex<fp_t> make_random_unit_complex()
 {
  fp_t r, i ;
  random_unit_complex(r,i) ;
  return Complex<fp_t>{r,i} ;
 }


//================================================================
// AoS
//================================================================

void init_aos( auto && collection )
 {
  using collection_type = std::remove_reference_t<decltype(collection)> ;
  using value_type = typename collection_type::value_type ;
  using fp_type = typename value_type::fp_type ;
  for ( auto & elem : collection )
   { elem = make_random_unit_complex<fp_type>() ; }
 }

// do not use std::span on this function,
// or the deep copy of init will not work.
void pow_aos( auto && collection, long long degree )
 {
  using collection_type = std::remove_reference_t<decltype(collection)> ;
  collection_type init {collection} ;
  for ( long long d = 1 ; d < degree ; ++d )
   {
    for ( std::size_t i = 0 ; i<collection.size() ; ++i )
     {
      collection[i] *= init[i] ;
     }
   }
 }

template< typename Collection >
auto reduce_aos( Collection && collection )
 {
  using collection_type = std::remove_reference_t<Collection> ;
  using value_type = typename collection_type::value_type ;
  using fp_type = typename value_type::fp_type ;
  value_type res  = make_unit_complex<fp_type>(1.,0.) ;
  for ( auto const & elem : collection )
   { res *= elem ; }
  return res ;
 }

template< std::floating_point fp_t >
class ComplexesAoS : public std::array<Complex<fp_t>,MAX_ARRAY_SIZE>
 {
  public :
    using array = std::array<Complex<fp_t>,MAX_ARRAY_SIZE> ;
    ComplexesAoS( std::size_t a_size ) : m_size(a_size)
     { init_aos(std::span<Complex<fp_t>>(this->begin(),m_size)) ; }
    void pow( long long degree )
     { pow_aos(*this,degree) ; }
    Complex<fp_t> reduce() const
     { return reduce_aos(std::span<Complex<fp_t> const>(this->begin(),m_size)) ; }
  private :
    std::size_t m_size ;
 } ;


//================================================================
// SoA of complex numbers
//================================================================

void random_soa( auto && reals, auto && imags, std::size_t size )
 {
  for ( std::size_t i = 0 ; i < size ; ++i )
   { random_unit_complex(reals[i],imags[i]) ; }
 }

void pow_soa( auto && reals, auto && imags, std::size_t size, long long degree )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  array_type r_init {reals}, i_init{imags} ;
  for ( long long d = 1 ; d < degree ; ++d )
   {
    for ( std::size_t i = 0 ; i<size ; ++i )
     {
      auto r_new = reals[i]*r_init[i] - imags[i]*i_init[i] ;
      auto i_new = imags[i]*r_init[i] + reals[i]*i_init[i] ;
      reals[i] = r_new ;
      imags[i] = i_new ;
     }
   }
 }

auto reduce_soa( auto const && reals, auto const && imags, std::size_t size )
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
    ComplexesSoA( std::size_t size ) : m_size(size), m_rs(), m_is()
     { random_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }
    void pow( long long degree )
     { pow_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size,degree) ; }
    auto reduce() const
     { return reduce_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    std::array<fp_t,MAX_ARRAY_SIZE> m_rs, m_is ;
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
