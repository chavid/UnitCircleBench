
#include <iostream>
#include <iomanip>
#include <cassert> // for assert
#include <cstdlib> // for rand
#include <valarray>

template< typename real >
struct XY
 { real x, y = 0. ; } ;

// We implement it as an independant function so that
// Maqao can look at it more easily. It can only work with
// kind of arrays which have a continuous memory area
template< typename real >
void saxpy( real * x, real * y, unsigned long long size, real a )
 {
  for ( auto i = 0ull ; i < size ; ++i )
   { y[i] =  a*x[i] + y[i] ; }
 }

template< typename Array >
class SoA
 {
  public :
    using real = typename Array::value_type ;
    SoA( unsigned long long size ) : xs_(size), ys_(size) {}
    XY<real> operator()( unsigned long long indice ) const
     { return { xs_[indice], ys_[indice] } ; }
    auto & xs() { return xs_ ; }
    auto & ys() { return ys_ ; }
    void saxpy( real a )
     { ::saxpy(&xs_[0],&ys_[0],xs_.size(),a) ; }
  private :
    Array xs_ ;
    Array ys_ ;
 } ;

template< typename Collection >
void randomize_x( Collection & collection )
 {
  srand(1) ;
  for ( auto & element : collection.xs() )
   { element = std::rand()/(RAND_MAX+1.)-0.5 ; }
 }

template< typename Collection >
typename Collection::real accumulate_y( Collection & collection, typename Collection::real init ) 
 {
  for ( auto element : collection.ys() )
   { init += element ; }
  return init ;
 }

template< typename real >
void main_impl( unsigned long long size, unsigned long long repeat )
 {
  SoA<std::valarray<real>> collection(size) ;
  randomize_x(collection) ;
  while (repeat--)
    collection.saxpy(static_cast<real>(0.1)) ;
  real res = accumulate_y(collection,static_cast<real>(0.))/size ;
  std::cout<<std::setw(2)<<sizeof(res)<<" octets : " <<res<<std::endl ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==4) ;
  std::string precision(argv[1]) ;
  unsigned long long size = atoi(argv[2]) ;
  unsigned long long repeat = atoi(argv[3]) ;
  std::cout.precision(18) ;

  if (precision=="float") main_impl<float>(size,repeat) ;
  else if (precision=="double") main_impl<double>(size,repeat) ;
  else if (precision=="long") main_impl<long double>(size,repeat) ;
  else throw "unknown precision" ;
 }
