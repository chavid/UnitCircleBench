
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
  long double e = (2.*std::numbers::pi_v<long double>*std::rand())/RAND_MAX ;
  cast_and_store(r,std::cosl(e)) ;
  cast_and_store(i,std::sinl(e)) ;
 }

template< std::floating_point fp_t >
class UnitComplex {

  public :

    using fp_type = fp_t ;

    UnitComplex( fp_t a_r, fp_t a_i ) : m_r{a_r}, m_i{a_i} {}
    UnitComplex() = default ;

    fp_t real() const { return m_r ; }
    fp_t imag() const { return m_i ; }
    fp_t magnitude() const { return std::sqrt(m_r*m_r+m_i*m_i) ; }
    fp_t argument() const { return std::atan(m_i/m_r) ; }

    UnitComplex & operator*=( UnitComplex const & other ) {
      auto r = m_r*other.m_r - m_i*other.m_i ;
      auto i = other.m_r*m_i + m_r*other.m_i ;
      m_r = r ;
      m_i = i ;
      return *this ;
    }
    friend UnitComplex operator*( UnitComplex const & lhs, UnitComplex const & rhs ) {
      auto r = lhs.m_r*rhs.m_r - lhs.m_i*rhs.m_i ;
      auto i = rhs.m_r*lhs.m_i + lhs.m_r*rhs.m_i ;
      return UnitComplex{r,i} ;
    }

  private :

    fp_t m_r {}, m_i {} ;

 } ;

template< std::floating_point fp_t1, std::floating_point fp_t2 >
UnitComplex<fp_t1> make_unit_complex( fp_t2 a_r, fp_t2 a_i )
 {
  fp_t1 r, i ;
  cast_and_store(r,a_r) ;
  cast_and_store(i,a_i) ;
  return UnitComplex<fp_t1>{r,i} ;
 }

template< std::floating_point fp_t >
UnitComplex<fp_t> make_random_unit_complex()
 {
  fp_t r, i ;
  random_unit_complex(r,i) ;
  return UnitComplex<fp_t>{r,i} ;
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

void pow_aos( auto && collection, long long degree )
 {
  for ( auto & elem : collection )
   {
    auto c = elem ;
    for ( long long d = 1 ; d < degree ; ++d )
     { elem *= c ; }
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

// default implementation using std:span
template< typename Array >
class AoS : public Array
 {
  public :
    using value_type = typename Array::value_type ;
    using fp_type = typename value_type::fp_type ;
    AoS( std::size_t a_size ) : Array(a_size)
     { init_aos(std::span<UnitComplex<fp_type>>(*this)) ; }
    void pow( long long degree )
     { pow_aos(std::span<UnitComplex<fp_type>>(*this),degree) ; }
    value_type reduce() const
     { return reduce_aos(std::span<UnitComplex<fp_type> const>(*this)) ; }
 } ;

// for C array, manage allocations  
template< std::floating_point fp_t >
class AoS<UnitComplex<fp_t>*>
 {
  public :
    using value_type = UnitComplex<fp_t> ;
    AoS( std::size_t a_size ) : m_size(a_size), m_data(new UnitComplex<fp_t>[a_size])
     { init_aos(std::span<UnitComplex<fp_t>>(m_data,m_size)) ; }
    void pow( long long degree )
     { pow_aos(std::span<UnitComplex<fp_t>>(m_data,m_size),degree) ; }
    UnitComplex<fp_t> reduce() const
     { return reduce_aos(std::span<UnitComplex<fp_t> const>(m_data,m_size)) ; }
  private :
    std::size_t m_size ;
    UnitComplex<fp_t> * m_data ;
 } ;

// for array, set explicitly the span size 
template< std::floating_point fp_t >
class AoS<std::array<UnitComplex<fp_t>,MAX_ARRAY_SIZE>> : public std::array<UnitComplex<fp_t>,MAX_ARRAY_SIZE>
 {
  public :
    using array = std::array<UnitComplex<fp_t>,MAX_ARRAY_SIZE> ;
    AoS( std::size_t a_size ) : m_size(a_size)
     { init_aos(std::span<UnitComplex<fp_t>>(this->begin(),m_size)) ; }
    void pow( long long degree )
     { pow_aos(std::span<UnitComplex<fp_t>>(this->begin(),m_size),degree) ; }
    UnitComplex<fp_t> reduce() const
     { return reduce_aos(std::span<UnitComplex<fp_t> const>(this->begin(),m_size)) ; }
  private :
    std::size_t m_size ;
 } ;

// for list, cannot use span 
template< std::floating_point fp_t >
class AoS<std::list<UnitComplex<fp_t>>> : public std::list<UnitComplex<fp_t>>
 {
  public :
    using list = std::list<UnitComplex<fp_t>> ;
    AoS( std::size_t a_size ) : list(a_size)
     { init_aos(*static_cast<list *>(this)) ; }
    void pow( long long degree )
     { pow_aos(*static_cast<list *>(this),degree) ; }
    UnitComplex<fp_t> reduce() const
     { return reduce_aos(*static_cast<list const *>(this)) ; }
 } ;



//================================================================
// SoA of complex numbers
//================================================================

void random_soa( auto && reals, auto && imags, std::size_t size )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  using fp_type = typename array_type::value_type ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   { random_unit_complex(reals[i],imags[i]) ; }
 }

void pow_soa( auto && reals, auto && imags, std::size_t size, long long degree )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  using fp_type = typename array_type::value_type ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   {
    UnitComplex<fp_type> cplx {reals[i],imags[i]}, res {cplx} ;
    for ( long long d = 1 ; d < degree ; ++d ) {
      res = res*cplx ;
    reals[i] = res.real() ;
    imags[i] = res.imag() ;
   }
  }
 }

auto reduce_soa( auto const && reals, auto const && imags, std::size_t size )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  using fp_type = typename array_type::value_type ;
  UnitComplex<fp_type> acc = make_unit_complex<fp_type>(1.,0.) ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   { acc *= UnitComplex<fp_type>{reals[i],imags[i]} ; }
  return acc ;
 }

template< typename Array >
class UnitComplexes
 {
  public :  
    UnitComplexes( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { random_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }
    void pow( long long degree )
     { pow_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size,degree) ; }
    auto reduce() const
     { return reduce_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    Array m_rs, m_is ;
 } ;

// for C array, manage memory
template< std::floating_point fp_t >
class UnitComplexes<fp_t *>
 {
  public :  
    UnitComplexes( std::size_t size ) : m_size(size), m_rs(new fp_t[size]), m_is(new fp_t[size])
     { random_soa(std::span(m_rs,m_size),std::span(m_is,m_size),m_size) ; }
    ~UnitComplexes() { delete [] m_rs ; delete [] m_is ; }
    void pow( long long degree )
     { pow_soa(std::span(m_rs,m_size),std::span(m_is,m_size),m_size,degree) ; }
    auto reduce() const
     { return reduce_soa(std::span(m_rs,m_size),std::span(m_is,m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    fp_t * m_rs, * m_is ;
 } ;

// for std::array, no size when constructing
template< std::floating_point fp_t >
class UnitComplexes<std::array<fp_t,MAX_ARRAY_SIZE>>
 {
  public :  
    UnitComplexes( std::size_t size ) : m_size(size), m_rs(), m_is()
     { random_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }
    void pow( long long degree )
     { pow_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size,degree) ; }
    auto reduce() const
     { return reduce_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    std::array<fp_t,MAX_ARRAY_SIZE> m_rs, m_is ;
 } ;

// for std::list, use iterators
template< std::floating_point fp_t >
class UnitComplexes<std::list<fp_t>>
 {
  public :  
    UnitComplexes( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     {
      for ( auto itr_rs = std::begin(m_rs), itr_is = std::begin(m_is) ; itr_rs != std::end(m_rs) ; ++itr_rs, ++itr_is )
       { random_unit_complex(*itr_rs,*itr_is) ; }
     }
    void pow( long long degree )
     {
      for ( auto itr_rs = std::begin(m_rs), itr_is = std::begin(m_is) ; itr_rs != std::end(m_rs) ; ++itr_rs, ++itr_is )
       {
        UnitComplex<fp_t> cplx {*itr_rs,*itr_is}, res {cplx} ;
        for ( long long d = 1 ; d < degree ; ++d )
         { res = res*cplx ; }
        *itr_rs = res.real() ;
        *itr_is = res.imag() ;
       }
     }
    auto reduce() const
     {
      UnitComplex<fp_t> acc = make_unit_complex<fp_t>(1.,0.) ;
      for ( auto itr_rs = std::begin(m_rs), itr_is = std::begin(m_is) ; itr_rs != std::end(m_rs) ; ++itr_rs, ++itr_is )
       {  acc *= UnitComplex<fp_t>{*itr_rs,*itr_is} ; }
      return acc ;
     }    
  private :
    std::size_t m_size ;
    std::list<fp_t> m_rs, m_is ;
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
  std::cout<<std::format("{} {}",res.magnitude(),res.argument())<<std::endl ;
 }

template< typename fp_t>
void main_aos( std::string & collection_tname, std::size_t size, long long degree )
 {
  if (collection_tname=="carray") main_impl<AoS<UnitComplex<fp_t>*>>(size,degree) ;
  else if (collection_tname=="array") main_impl<AoS<std::array<UnitComplex<fp_t>,MAX_ARRAY_SIZE>>>(size,degree) ;
  else if (collection_tname=="dynarray") main_impl<AoS<DynArray<UnitComplex<fp_t>>>>(size,degree) ;
  else if (collection_tname=="valarray") main_impl<AoS<std::valarray<UnitComplex<fp_t>>>>(size,degree) ;
  else if (collection_tname=="vector") main_impl<AoS<std::vector<UnitComplex<fp_t>>>>(size,degree) ;
  else if (collection_tname=="list") main_impl<AoS<std::list<UnitComplex<fp_t>>>>(size,degree) ;
  else throw "unknown collection_tname" ;
 }

template< typename fp_t>
void main_soa( std::string & collection_tname, std::size_t size, long long degree )
 {
  if (collection_tname=="carray") main_impl<UnitComplexes<fp_t *>>(size,degree) ;
  else if (collection_tname=="array") main_impl<UnitComplexes<std::array<fp_t,MAX_ARRAY_SIZE>>>(size,degree) ;
  else if (collection_tname=="dynarray") main_impl<UnitComplexes<DynArray<fp_t>>>(size,degree) ;
  else if (collection_tname=="valarray") main_impl<UnitComplexes<std::valarray<fp_t>>>(size,degree) ;
  else if (collection_tname=="vector") main_impl<UnitComplexes<std::vector<fp_t>>>(size,degree) ;
  else if (collection_tname=="list") main_impl<UnitComplexes<std::list<fp_t>>>(size,degree) ;
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
