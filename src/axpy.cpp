
#include <iostream>
#include <cassert> // for assert
#include <cstdlib> // for rand
#include <valarray>
#include <array>
#include <list>
#include <vector>
#include <stdfloat>

#include "utilities.hh"


//================================================================
// AoS
//================================================================

template< std::floating_point fp_t >
struct XY
 {
  using fp_type = fp_t ;
  XY()
   {
    x = std::rand()/(RAND_MAX+static_cast<fp_t>(1.0))-static_cast<fp_t>(0.5) ;
    y = static_cast<fp_t>(0.) ;
   }
  fp_t x, y {} ;
  void axpy( fp_t a )
   { y = a*x + y ; }
 } ;

template< typename Array >
class AoS : public Array
 {
  public :
    using value_type = typename Array::value_type ;
    using fp_type = typename value_type::fp_type ;
    using Array::Array ;
    void axpy( fp_type a )
     {
      for ( auto & elem : *this )
       { elem.axpy(a) ; }
     }
    fp_type mean_y() const
     {
      fp_type res = static_cast<fp_type>(0.) ;
      for ( auto const & elem : *this )
       { res += elem.y ; }
      return res/this->size() ;
     }
 } ;

template< std::floating_point fp_t >
class AoS<std::array<XY<fp_t>,65536>>
 {
  public :
    using fp_type = fp_t ;
    AoS( std::size_t size ) : m_size(size) {}
    std::size_t size() const { return m_size ; }
    void axpy( fp_type a )
     {
      for ( std::size_t i {} ; i<m_size ; ++i )
       { m_xys[i].axpy(a) ; }
     }
    fp_type mean_y() const
     {
      fp_type res = static_cast<fp_type>(0.) ;
      for ( std::size_t i {} ; i<m_size ; ++i )
       { res += m_xys[i].y ; }
      return res/m_size ;
     }
  private :
    std::size_t m_size ;
    std::array<XY<fp_t>,65536> m_xys ;
 } ;

template< std::floating_point fp_t >
class AoS<XY<fp_t>*>
 {
  public :
    using fp_type = fp_t ;
    AoS( std::size_t size ) : m_xys(new XY<fp_t>[size]), m_size{size} {}
    ~AoS() { delete [] m_xys ; }
    std::size_t size() const { return m_size ; }
    void axpy( fp_type a )
     {
      for ( std::size_t i {} ; i<m_size ; ++i )
       { m_xys[i].axpy(a) ; }
     }
    fp_type mean_y() const
     {
      fp_type res = static_cast<fp_type>(0.) ;
      for ( std::size_t i {} ; i<m_size ; ++i )
       { res += m_xys[i].y ; }
      return res/m_size ;
     }
  private :
    XY<fp_t> * m_xys ;
    std::size_t m_size ;
 } ;


//================================================================
// SoA
//================================================================

template< std::floating_point fp_t >
class SoaBase
 {
  public :
    using fp_type = fp_t ;
    SoaBase( std::size_t size ) : m_size(size) {}
    template< typename Array >
    void init( Array & xs, Array & ys ) const
     {
      for ( std::size_t i {} ; i < m_size ; ++i )
       {
        xs[i] = std::rand()/(RAND_MAX+static_cast<fp_type>(1.0))-static_cast<fp_type>(0.5) ;
        ys[i] = static_cast<fp_type>(0.) ;
       }
     }
    template< typename Array >
    void axpy_impl( Array & xs, Array & ys, fp_type a )
     {
      for ( std::size_t i {} ; i < m_size ; ++i )
       { ys[i] += a*xs[i] ; }
     }
    template< typename Array >
    fp_type mean_y_impl( Array & ys ) const
     {
      fp_type sum {static_cast<fp_type>(0.)} ;
      for ( std::size_t i {} ; i < m_size ; ++i )
       { sum += ys[i] ; }
      return sum/m_size ;
     }
  protected :
    std::size_t m_size ;
 } ;

// default for std::vector and DynArray
template< typename Array >
class SoA : private SoaBase<typename Array::value_type>
 {
  public :
    using fp_type = typename Array::value_type ;
    SoA( std::size_t size ) : SoaBase<typename Array::value_type>(size), m_xs(size), m_ys(size)
     { this->init(m_xs,m_ys) ; }
    void axpy( fp_type a )
     { this->axpy_impl(m_xs,m_ys,a) ; }
    fp_type mean_y() const
     { return this->mean_y_impl(m_ys) ; }
  private :
    Array m_xs ;
    Array m_ys ;
 } ;

// for C raw pointers, make the new/delete
template< std::floating_point fp_t >
class SoA<fp_t *> : private SoaBase<fp_t>
 {
  public :
    using fp_type = fp_t ;
    SoA( std::size_t size ) : SoaBase<fp_t>(size), m_xs(new fp_t [size]), m_ys(new fp_t [size])
     { this->init(m_xs,m_ys) ; }
    ~SoA() { delete [] m_xs ; delete [] m_ys ; }
    void axpy( fp_t a )
     { this->axpy_impl(m_xs,m_ys,a) ; }
    fp_t mean_y() const
     { return this->mean_y_impl(m_ys) ; }
  private :
    fp_t * m_xs ;
    fp_t * m_ys ;
 } ;

// for std::array, preallocate the maximum size
template< std::floating_point fp_t >
class SoA<std::array<fp_t,65536>> : private SoaBase<fp_t>
 {
  public :
    using fp_type = fp_t ;
    SoA( std::size_t size ) : SoaBase<fp_t>(size)
     { this->init(m_xs,m_ys) ; }
    void axpy( fp_t a )
     { this->axpy_impl(m_xs,m_ys,a) ; }
    fp_t mean_y() const
     { return this->mean_y_impl(m_ys) ; }
  private :
    std::array<fp_t,65536> m_xs ;
    std::array<fp_t,65536> m_ys ;
 } ;

// for valarray, use the global operators
template< std::floating_point fp_t >
class SoA<std::valarray<fp_t>> : private SoaBase<fp_t>
 {
  public :
    using fp_type = fp_t ;
    SoA( std::size_t size ) : SoaBase<fp_t>(size), m_xs(size), m_ys(size)
     { this->init(m_xs,m_ys) ; }
    void axpy( fp_t a )
     {  m_ys += a*m_xs ; }
    fp_t mean_y() const
     { return this->mean_y_impl(m_ys) ; }
  private :
    std::valarray<fp_t> m_xs ;
    std::valarray<fp_t> m_ys ;
 } ;

// for list, cannot use operator []
template< std::floating_point fp_t >
class SoA<std::list<fp_t>>
 {
  public :
    using fp_type = fp_t ;
    SoA( std::size_t size ) : m_xs(size), m_ys(size)
     {
      auto xs = m_xs.begin() ;
      auto end = m_xs.end() ;
      auto ys = m_ys.begin() ;
      for ( ; xs != end ; ++xs, ++ys )
       {
        (*xs) = std::rand()/(RAND_MAX+static_cast<fp_type>(1.0))-static_cast<fp_type>(0.5) ;
        (*ys) = static_cast<fp_type>(0.) ;
       }
     }
    void axpy( fp_type a )
     {
      auto xs = m_xs.begin() ;
      auto end = m_xs.end() ;
      auto ys = m_ys.begin() ;
      for ( ; xs != end ; ++xs, ++ys )
       { (*ys) = a*(*xs) + (*ys) ; }
     }
    fp_type mean_y() const
     {
      fp_type sum {static_cast<fp_type>(0.)} ;
      for ( auto ys = m_ys.begin() ; ys != m_ys.end() ; ++ys )
       { sum += (*ys) ; }
      return sum/m_ys.size() ;
     }
  private :
    std::list<fp_t> m_xs ;
    std::list<fp_t> m_ys ;
 } ;


//================================================================
// Main
//================================================================

template< typename Collection >
void main_impl( std::size_t size, std::size_t repeat )
 {
  srand(1) ;
  using fp_type = typename Collection::fp_type ;
  Collection collection(size) ;
  auto volatile a {static_cast<fp_type>(0.1)} ; // volatile prevent the compiler from optimizing the repetition
  while (repeat--)
    collection.axpy(a) ;
  fp_type res = collection.mean_y() ;
  std::cout<<myformat("Precision: {:2d} bytes",sizeof(res))<<std::endl ;
  std::cout<<myformat("Result: {}",res)<<std::endl ;
  std::cout<<myformat("(checksum: {})",res)<<std::endl ;
 }

template< std::floating_point fp_t >
void main_aos( std::string & collection_name, std::size_t size, std::size_t repeat )
 {
  if (collection_name=="carray") main_impl<AoS<XY<fp_t>*>>(size,repeat) ;
  else if (collection_name=="array") main_impl<AoS<std::array<XY<fp_t>,65536>>>(size,repeat) ;
  else if (collection_name=="dynarray") main_impl<AoS<DynArray<XY<fp_t>>>>(size,repeat) ;
  else if (collection_name=="valarray") main_impl<AoS<std::valarray<XY<fp_t>>>>(size,repeat) ;
  else if (collection_name=="vector") main_impl<AoS<std::vector<XY<fp_t>>>>(size,repeat) ;
  else if (collection_name=="list") main_impl<AoS<std::list<XY<fp_t>>>>(size,repeat) ;
  else throw "unknown collection_name" ;
 }

template< std::floating_point fp_t >
void main_soa( std::string & collection_name, std::size_t size, std::size_t repeat )
 {
  if (collection_name=="carray") main_impl<SoA<fp_t *>>(size,repeat) ;
  else if (collection_name=="array") main_impl<SoA<std::array<fp_t,65536>>>(size,repeat) ;
  else if (collection_name=="dynarray") main_impl<SoA<DynArray<fp_t>>>(size,repeat) ;
  else if (collection_name=="valarray") main_impl<SoA<std::valarray<fp_t>>>(size,repeat) ;
  else if (collection_name=="vector") main_impl<SoA<std::vector<fp_t>>>(size,repeat) ;
  else if (collection_name=="list") main_impl<SoA<std::list<fp_t>>>(size,repeat) ;
  else throw "unknown collection_name" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==6) ;
  std::string arrangement_name(argv[1]) ;
  std::string collection_name(argv[2]) ;
  std::string fp_name(argv[3]) ;
  std::size_t size {std::strtoull(argv[4],nullptr,10)} ;
  std::size_t repeat {std::strtoull(argv[5],nullptr,10)} ;

  if (arrangement_name=="aos")
   {
    if (fp_name=="half") main_aos<std::float16_t>(collection_name,size,repeat) ;
    else if (fp_name=="float") main_aos<float>(collection_name,size,repeat) ;
    else if (fp_name=="double") main_aos<double>(collection_name,size,repeat) ;
    else if (fp_name=="long") main_aos<long double>(collection_name,size,repeat) ;
    else if (fp_name=="quad") main_aos<std::float128_t>(collection_name,size,repeat) ;
    else throw "unknown fp_name" ;
   }
  else if (arrangement_name=="soa")
   {
    if (fp_name=="half") main_soa<std::float16_t>(collection_name,size,repeat) ;
    else if (fp_name=="float") main_soa<float>(collection_name,size,repeat) ;
    else if (fp_name=="double") main_soa<double>(collection_name,size,repeat) ;
    else if (fp_name=="long") main_soa<long double>(collection_name,size,repeat) ;
    else if (fp_name=="quad") main_soa<std::float128_t>(collection_name,size,repeat) ;
    else throw "unknown fp_name" ;
   }
  else throw "unknown arrangement_name" ;
 }
