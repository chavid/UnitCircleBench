#include <iostream>
#include <iomanip>
#include <cassert> // for assert
#include <cstdlib> // for rand
#include <valarray>
#include <vector>

template< typename real >
class DynArray
 {
  public :
    using value_type = real ;
    explicit DynArray( unsigned long long size ) : size_(size), data_(new real [size]) {}
    real * begin() { return data_ ; }
    real * end() { return data_+size_ ; }
    real const * begin() const { return data_ ; }
    real const * end() const { return data_+size_ ; }
    unsigned long long size() { return size_ ; }
    real & operator[]( unsigned long long indice ) { return data_[indice] ; }
    real const & operator[]( unsigned long long indice ) const { return data_[indice] ; }
    ~DynArray() { delete [] data_ ; }
  private :
    unsigned long long size_ ;
    real * data_ ;
 } ;

template< typename real >
struct XY
 {
  using value_type = real ;
  real x, y = 0. ;
  void saxpy( real a )
   { y = a*x + y ; }
 } ;

template< typename Collection >
void randomize_x( Collection & collection )
 {
  srand(1) ;
  for ( auto & elem : collection )
   { elem.x = std::rand()/(RAND_MAX+1.)-0.5 ; }
 }

template< typename Collection, typename real >
void saxpy( Collection & collection, real a )
 {
  for ( auto & elem : collection )
   { elem.saxpy(a) ; }
 }

template< typename Collection, typename real >
real accumulate_y( Collection const & collection, real init )
 {
  for ( auto const & elem : collection )
   { init += elem.y ; }
  return init ;
 }

template< typename Collection >
void main_impl( unsigned long long size, unsigned long long repeat )
 {
  using xy = typename Collection::value_type ;
  using real = typename xy::value_type ;
  Collection collection(size) ;
  randomize_x(collection) ;
  while (repeat--)
    saxpy(collection,static_cast<real>(0.1)) ;
  real res = accumulate_y(collection,static_cast<real>(0.))/size ;
  std::cout<<std::setw(2)<<sizeof(res)<<" octets : " <<res<<std::endl ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==4) ;
  std::string impl(argv[1]) ;
  unsigned long long size = atoi(argv[2]) ;
  unsigned long long repeat = atoi(argv[3]) ;
  std::cout.precision(18) ;

  if (impl=="float") main_impl<std::valarray<XY<float>>>(size,repeat) ;
  else if (impl=="double") main_impl<std::valarray<XY<double>>>(size,repeat) ;
  else if (impl=="long") main_impl<std::valarray<XY<long double>>>(size,repeat) ;
  else if (impl=="dynarray") main_impl<DynArray<XY<double>>>(size,repeat) ;
  else if (impl=="valarray") main_impl<std::valarray<XY<double>>>(size,repeat) ;
  else if (impl=="vector") main_impl<std::vector<XY<double>>>(size,repeat) ;
  else throw "unknown impl" ;
 }
