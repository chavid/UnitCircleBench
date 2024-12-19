
#include <iostream>
#include <iomanip>
#include <cassert> // for assert
#include <cstdlib> // for rand
#include <valarray>
#include <vector>
#include <stdfloat>
#include <format>


//================================================================
// Utilities
//================================================================

template < typename T > class TypeDisplayer ;

// Home-made array which is stored in the heap
template< typename T >
class DynArray
 {
  public :
    using value_type = T ;
    explicit DynArray( std::size_t size ) : size_(size), data_(new T [size]) {}
    T * begin() { return data_ ; }
    T * end() { return data_+size_ ; }
    T const * begin() const { return data_ ; }
    T const * end() const { return data_+size_ ; }
    std::size_t size() { return size_ ; }
    T & operator[]( std::size_t indice ) { return data_[indice] ; }
    T const & operator[]( std::size_t indice ) const { return data_[indice] ; }
    ~DynArray() { delete [] data_ ; }
  private :
    std::size_t size_ ;
    T * data_ ;
 } ;


//================================================================
// AoS
//================================================================

template< std::floating_point fp_t >
struct XY
 {
  using value_type = fp_t ;
  fp_t x, y {} ;
  void saxpy( fp_t a )
   { y = a*x + y ; }
 } ;

template< typename Array >
class AoS : public Array
 {
  public :
    using xy = typename Array::value_type ;
    using fp_t = typename xy::value_type ;
    using Array::Array ;
    auto & x( std::size_t indice ) { return this->operator[](indice).x ; }
    auto & y( std::size_t indice ) { return this->operator[](indice).y ; }
 } ;

template< typename Array >
void randomize_x( AoS<Array> & collection )
 {
  using fp_t = typename AoS<Array>::fp_t ;
  srand(1) ;
  for ( auto & elem : collection )
   { elem.x = std::rand()/(RAND_MAX+static_cast<fp_t>(1.0))-static_cast<fp_t>(-0.5) ; }
 }

template< typename Array >
void saxpy( AoS<Array> & collection, typename AoS<Array>::fp_t a )
 {
  for ( auto & elem : collection )
   { elem.saxpy(a) ; }
 }

template< typename Array >
AoS<Array>::fp_t accumulate_y( AoS<Array> const & collection, typename AoS<Array>::fp_t init )
 {
  for ( auto const & elem : collection )
   { init += elem.y ; }
  return init ;
 }


//================================================================
// SoA
//================================================================

// We implement this as an independant function so that
// Maqao can look at it more easily. It can only work with
// kind of arrays which have a continuous memory area
template< std::floating_point fp_t >
void saxpy( fp_t * x, fp_t * y, std::size_t size, fp_t a )
 {
  for ( std::size_t i = 0ull ; i < size ; ++i )
   { y[i] =  a*x[i] + y[i] ; }
 }

template< typename Array >
class SoA
 {
  public :
    using fp_t = typename Array::value_type ;
    SoA( std::size_t size ) : xs_(size), ys_(size) {}
    XY<fp_t> operator()( std::size_t indice ) const
     { return { xs_[indice], ys_[indice] } ; }
    auto & xs() { return xs_ ; }
    auto & ys() { return ys_ ; }
    void saxpy( fp_t a )
     { ::saxpy(&xs_[0],&ys_[0],xs_.size(),a) ; }
  private :
    Array xs_ ;
    Array ys_ ;
 } ;

template< typename Array >
void randomize_x( SoA<Array> & collection )
 {
  using fp_t = typename SoA<Array>::fp_t ;
  srand(1) ;
  for ( auto & element : collection.xs() )
   { element = std::rand()/(RAND_MAX+static_cast<fp_t>(1.0))-static_cast<fp_t>(-0.5) ; }
 }

template< typename Array >
void saxpy( SoA<Array> & xys, typename SoA<Array>::fp_t a )
 {
  for ( std::size_t i {} ; i < xys.xs().size() ; ++i )
   { xys.ys()[i] =  a*xys.xs()[i] + xys.ys()[i] ; }
 }

template< typename Array >
typename SoA<Array>::fp_t accumulate_y( SoA<Array> & collection, typename SoA<Array>::fp_t init ) 
 {
  for ( auto element : collection.ys() )
   { init += element ; }
  return init ;
 }


//================================================================
// Main
//================================================================

template< typename Collection >
void main_impl( std::size_t size, std::size_t repeat )
 {
  using fp_t = typename Collection::fp_t ;
  Collection collection(size) ;
  randomize_x(collection) ;
  while (repeat--)
    saxpy(collection,static_cast<fp_t>(0.1)) ;
  fp_t res = accumulate_y(collection,static_cast<fp_t>(0.))/size ;
  std::cout<<std::setw(2)<<sizeof(res)<<" octets : " <<std::format("{}",res)<<std::endl ;
 }

template< std::floating_point fp_t >
void main_aos( std::string & collection_tname, std::size_t size, std::size_t repeat )
 {
  if (collection_tname=="dynarray") main_impl<AoS<DynArray<XY<fp_t>>>>(size,repeat) ;
  else if (collection_tname=="valarray") main_impl<AoS<std::valarray<XY<fp_t>>>>(size,repeat) ;
  else if (collection_tname=="vector") main_impl<AoS<std::vector<XY<fp_t>>>>(size,repeat) ;
  else throw "unknown collection_tname" ;
 }

template< std::floating_point fp_t >
void main_soa( std::string & collection_tname, std::size_t size, std::size_t repeat )
 {
  if (collection_tname=="dynarray") main_impl<SoA<DynArray<fp_t>>>(size,repeat) ;
  else if (collection_tname=="valarray") main_impl<SoA<std::valarray<fp_t>>>(size,repeat) ;
  else if (collection_tname=="vector") main_impl<SoA<std::vector<fp_t>>>(size,repeat) ;
  else throw "unknown collection_tname" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==6) ;
  std::string arrangement_tname(argv[1]) ;
  std::string collection_tname(argv[2]) ;
  std::string fp_tname(argv[3]) ;
  std::size_t size = atoi(argv[4]) ;
  std::size_t repeat = atoi(argv[5]) ;

  if (arrangement_tname=="aos")
   {
    if (fp_tname=="half") main_aos<std::float16_t>(collection_tname,size,repeat) ;
    else if (fp_tname=="float") main_aos<float>(collection_tname,size,repeat) ;
    else if (fp_tname=="double") main_aos<double>(collection_tname,size,repeat) ;
    else if (fp_tname=="long") main_aos<long double>(collection_tname,size,repeat) ;
    else if (fp_tname=="quad") main_aos<std::float128_t>(collection_tname,size,repeat) ;
    else throw "unknown fp_tname" ;
   }
  else if (arrangement_tname=="soa")
   {
    if (fp_tname=="half") main_soa<std::float16_t>(collection_tname,size,repeat) ;
    else if (fp_tname=="float") main_soa<float>(collection_tname,size,repeat) ;
    else if (fp_tname=="double") main_soa<double>(collection_tname,size,repeat) ;
    else if (fp_tname=="long") main_soa<long double>(collection_tname,size,repeat) ;
    else if (fp_tname=="quad") main_soa<std::float128_t>(collection_tname,size,repeat) ;
    else throw "unknown fp_tname" ;
   }
  else throw "unknown arrangement_tname" ;
 }
