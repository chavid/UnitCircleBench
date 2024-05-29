
#include <iostream>
#include <cassert> // for assert
#include <cstdlib> // for rand
#include <valarray>
#include <stdfloat>
#include <complex>
#include <cmath>

// SoA of complex numbers
template< typename R >
class Complexes {
  public :  
  
    Complexes( std::size_t size ) : m_rs(size), m_is(size) {}
    std::size_t size() const { return m_rs.size() ; }
    
    std::complex<R> operator[]( std::size_t index ) const
     { return { m_rs[index], m_is[index] } ; }
    void real( std::size_t index, R value ) { m_rs[index] = value ; }
    void imag( std::size_t index, R value ) { m_is[index] = value ; }
    
    friend Complexes<R> operator*( Complexes<R> const & lhs, Complexes<R> const & rhs ) {
      Complexes<R> res {lhs.size()} ;
      res.m_rs = lhs.m_rs*rhs.m_rs - lhs.m_is*rhs.m_is ;
      res.m_is = rhs.m_rs*lhs.m_is + lhs.m_rs*rhs.m_is ;
      return res ;
    }
    
  private :
    std::valarray<R> m_rs, m_is ;
 } ;

template< typename R >
Complexes<R> random( std::size_t size )
 {
  srand(1) ;
  Complexes<R> cplxs {size} ;
  for ( std::size_t i = 0 ; i < cplxs.size() ; ++i )
   {
    long double e = 2*M_PI*(static_cast<long double>(std::rand())/RAND_MAX) ;
    cplxs.real(i,static_cast<R>(std::cos(e))) ;
    cplxs.imag(i,static_cast<R>(std::sin(e))) ;
   }
  return cplxs ;
 }

template< typename R>
Complexes<R> pow( Complexes<R> const & cplxs, long long degree )
 {
  Complexes<R> res {cplxs} ;
  for ( long long d = 1 ; d < degree ; ++d ) {
    res = res*cplxs ;
  }
  return res ;
 }

template< typename R>
std::complex<R> reduce( Complexes<R> const & cplxs )
 {
  std::complex<R> acc { static_cast<R>(1.), static_cast<R>(0.) } ;
  for ( std::size_t i = 0 ; i < cplxs.size() ; ++i )
   { acc *= cplxs[i] ; }
  return acc ;
 }

template< typename R>
void main_impl( std::size_t size, long long degree )
 {
  std::complex<R> res = reduce(pow(random<R>(size),degree)) ;
  R re = res.real(), im = res.imag() ;
  auto r = std::sqrt(static_cast<long double>(re*re+im*im)) ;
  auto n = std::atan(static_cast<long double>(im/re)) ;
  std::cout<<r<<" "<<n<<std::endl ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==4) ;
  std::string precision(argv[1]) ;
  std::size_t size = atoi(argv[2]) ;
  long long degree = atoll(argv[3]) ;
  std::cout.precision(18) ;

  if (precision=="half") main_impl<std::float16_t>(size,degree) ;
  else if (precision=="float") main_impl<float>(size,degree) ;
  else if (precision=="double") main_impl<double>(size,degree) ;
  else if (precision=="long") main_impl<long double>(size,degree) ;
  else if (precision=="quad") main_impl<std::float128_t>(size,degree) ;
  else throw "unknown precision" ;
 }
