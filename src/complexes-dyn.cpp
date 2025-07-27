

#include <iostream>
#include <cassert> // for assert
#include <valarray>
#include <vector>
#include <list>
#include <array>
#include <stdfloat>
#include <numbers>
#include <cmath>
#include <format>

#include "utilities.hh"
#include "unit-complex.hh"


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

template< std::floating_point fp_t >
void ax_aos( Complex<fp_t> * xs, Complex<fp_t> const * as, std::size_t size, long long repeat )
 {
  for ( long long d = 1 ; d < repeat ; ++d )
   {
    for ( std::size_t i = 0 ; i<size ; ++i )
     { xs[i] *= as[i] ; }
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

// default implementation
template< typename Array >
class AoS : public Array
 {
  public :
    using value_type = typename Array::value_type ;
    using fp_type = typename value_type::fp_type ;
    AoS( std::size_t a_size ) : Array(a_size)
     { init_aos(std::span<Complex<fp_type>>(*this)) ; }
    void pow( long long degree )
     {
      Array as{*this} ;
      time("pow",ax_aos<fp_type>,this->data(),as.data(),this->size(),degree) ;
     }
    value_type reduce() const
     { return reduce_aos(std::span<Complex<fp_type> const>(*this)) ; }
 } ;

// for C array, manage allocations  
template< std::floating_point fp_t >
class AoS<Complex<fp_t>*>
 {
  public :
    using value_type = Complex<fp_t> ;
    explicit AoS( std::size_t a_size ) : m_size(a_size), m_data(new Complex<fp_t>[a_size])
     { init_aos(std::span<Complex<fp_t>>(m_data,m_size)) ; }
    AoS( AoS const & ) = delete ;
    AoS( AoS && other ) : m_size(other.m_size),m_data(other.m_data)
     { other.m_data = nullptr ; }
    AoS & operator=( AoS const & ) = delete ;
    AoS & operator=( AoS && other )
     {
      m_size = other.m_size ;
      m_data = other.m_data ;
      other.m_data = nullptr ;
      return *this ;
     }
    ~AoS()
     { delete [] m_data ; }
    void pow( long long degree )
     {
      auto as = new Complex<fp_t> [m_size] ;
      std::copy(m_data,m_data+m_size,as) ;
      time("pow",ax_aos<fp_t>,m_data,as,m_size,degree) ;
      delete [] as ;
     }
    Complex<fp_t> reduce() const
     { return reduce_aos(std::span<Complex<fp_t> const>(m_data,m_size)) ; }
  private :
    std::size_t m_size ;
    Complex<fp_t> * m_data ;
 } ;

// for array, set explicitly the span size 
template< std::floating_point fp_t >
class AoS<std::array<Complex<fp_t>,MAX_ARRAY_SIZE>> : public std::array<Complex<fp_t>,MAX_ARRAY_SIZE>
 {
  public :
    using array = std::array<Complex<fp_t>,MAX_ARRAY_SIZE> ;
    AoS( std::size_t a_size ) : m_size(a_size)
     { init_aos(std::span<Complex<fp_t>>(this->begin(),m_size)) ; }
    void pow( long long degree )
     {
      auto as {*this} ;
      time("pow",ax_aos<fp_t>,this->data(),as.data(),m_size,degree) ;
     }
    Complex<fp_t> reduce() const
     { return reduce_aos(std::span<Complex<fp_t> const>(this->begin(),m_size)) ; }
  private :
    std::size_t m_size ;
 } ;

// for valarray, no internal loop

template< std::floating_point fp_t >
void pow_aos_valarray( std::valarray<Complex<fp_t>> & collection, long long degree )
 {
  auto const init {collection} ;
  for ( long long d = 1 ; d < degree ; ++d )
   { collection *= init ; }
 }

template< std::floating_point fp_t >
class AoS<std::valarray<Complex<fp_t>>> : public std::valarray<Complex<fp_t>>
 {
  public :
    using valarray = std::valarray<Complex<fp_t>> ;
    using value_type = Complex<fp_t> ;
    using fp_type = typename value_type::fp_type ;
    AoS( std::size_t a_size ) : valarray(a_size)
     { init_aos(std::span<Complex<fp_type>>(*this)) ; }
    void pow( long long degree )
     { time("pow",pow_aos_valarray<fp_type>,*this,degree) ; }
    value_type reduce() const
     { return reduce_aos(std::span<Complex<fp_type> const>(*this)) ; }
 } ;

// for list, cannot use neither span nor []

template< std::floating_point fp_t >
void pow_aos_list( std::list<Complex<fp_t>> & collection, long long degree )
 {
  for ( auto & element : collection )
   {
    auto init = element ;
    for ( long long d = 1 ; d < degree ; ++d )
     { element *= init ; }
   }
 }

template< std::floating_point fp_t >
class AoS<std::list<Complex<fp_t>>> : public std::list<Complex<fp_t>>
 {
  public :
    using list = std::list<Complex<fp_t>> ;
    AoS( std::size_t a_size ) : list(a_size)
     { init_aos(*static_cast<list *>(this)) ; }
    void pow( long long degree )
     { time("pow",pow_aos_list<fp_t>,*this,degree) ; }
    Complex<fp_t> reduce() const
     { return reduce_aos(*static_cast<list const *>(this)) ; }
 } ;



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
  fp_t __restrict__ * xreals, fp_t __restrict__ * ximags,
  fp_t const __restrict__ * areals, fp_t const __restrict__ * aimags, 
  std::size_t size, long long repeat )
 {
  for ( long long d = 1 ; d < repeat ; ++d )
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

auto reduce_soa( auto const && reals, auto const && imags, std::size_t size )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  using fp_type = typename array_type::value_type ;
  Complex<fp_type> acc = make_unit_complex<fp_type>(1.,0.) ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   { acc *= Complex<fp_type>{reals[i],imags[i]} ; }
  return acc ;
 }

template< typename Array >
class Complexes
 {
  public :  
    Complexes( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { random_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }
    void pow( long long degree )
     {
      using fp_type = typename Array::value_type ;
      Array a_rs{m_rs}, a_is{m_is} ;
      time("pow",ax_soa<fp_type>,m_rs.data(),m_is.data(),a_rs.data(),a_is.data(),m_size,degree) ;
    }
    auto reduce() const
     { return reduce_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    Array m_rs, m_is ;
 } ;

// for C array, manage memory

template< std::floating_point fp_t >
class Complexes<fp_t *>
 {
  public :  
    explicit Complexes( std::size_t size ) : m_size(size), m_rs(new fp_t[size]), m_is(new fp_t[size])
     { random_soa(std::span(m_rs,m_size),std::span(m_is,m_size),m_size) ; }
    Complexes( Complexes const & ) = delete ;
    Complexes( Complexes && other ) : m_size(other.m_size),m_rs(other.m_rs), m_is(other.m_is)
     { other.m_rs = nullptr ; other.m_is = nullptr ; }
    Complexes & operator=( Complexes const & ) = delete ;
    Complexes & operator=( Complexes && other )
     {
      m_size = other.m_size ;
      m_rs = other.m_rs ;
      other.m_rs = nullptr ;
      m_is = other.m_is ;
      other.m_is = nullptr ;
      return *this ;
     }
    ~Complexes() { delete [] m_rs ; delete [] m_is ; }
    void pow( long long degree )
     {
      auto r_init = new fp_t [m_size] ;
      std::copy(m_rs,m_rs+m_size,r_init) ;
      auto i_init = new fp_t [m_size] ;
      std::copy(m_is,m_is+m_size,i_init) ;
      time("pow",ax_soa<fp_t>,m_rs,m_is,r_init,i_init,m_size,degree) ;
      delete [] r_init ;
      delete [] i_init ;
     }
    auto reduce() const
     { return reduce_soa(std::span(m_rs,m_size),std::span(m_is,m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    fp_t * m_rs, * m_is ;
 } ;

// for std::array, no size when constructing

template< std::floating_point fp_t >
class Complexes<std::array<fp_t,MAX_ARRAY_SIZE>>
 {
  public :  
    Complexes( std::size_t size ) : m_size(size), m_rs(), m_is()
     { random_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }
    void pow( long long degree )
     {
      auto r_init{m_rs}, i_init{m_is} ;
      time("pow",ax_soa<fp_t>,m_rs.data(),m_is.data(),r_init.data(),i_init.data(),m_size,degree) ;
     }
    auto reduce() const
     { return reduce_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    std::array<fp_t,MAX_ARRAY_SIZE> m_rs, m_is ;
 } ;

 // for valarray, no internal loop

template< std::floating_point fp_t >
void pow_soa_valarray(
  std::valarray<fp_t> & xreals, std::valarray<fp_t> & ximags,
  long long repeat )
 {
  auto const areals {xreals} ;
  auto const aimags {ximags} ;
  std::valarray<fp_t> r_new, i_new ;
  for ( long long d = 1 ; d < repeat ; ++d )
   {
    r_new = xreals*areals - ximags*aimags ;
    i_new = ximags*areals + xreals*aimags ;
    xreals = r_new ;
    ximags = i_new ;
   }
 }

template< std::floating_point fp_t >
class Complexes<std::valarray<fp_t>>
 {
  public :  
    Complexes( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { random_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }
    void pow( long long degree )
      { time("pow",pow_soa_valarray<fp_t>,m_rs,m_is,degree) ; }
    auto reduce() const
     { return reduce_soa(std::span(std::begin(m_rs),m_size),std::span(std::begin(m_is),m_size),m_size) ; }    
  private :
    std::size_t m_size ;
    std::valarray<fp_t> m_rs, m_is ;
 } ;

// for std::list, use iterators

template< std::floating_point fp_t >
class Complexes<std::list<fp_t>>
 {
  public :  
    Complexes( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     {
      for ( auto itr_rs = std::begin(m_rs), itr_is = std::begin(m_is) ; itr_rs != std::end(m_rs) ; ++itr_rs, ++itr_is )
       { random_unit_complex(*itr_rs,*itr_is) ; }
     }
    void pow( long long degree )
     {
      for ( auto itr_rs = std::begin(m_rs), itr_is = std::begin(m_is) ; itr_rs != std::end(m_rs) ; ++itr_rs, ++itr_is )
       {
        Complex<fp_t> cplx {*itr_rs,*itr_is}, res {cplx} ;
        for ( long long d = 1 ; d < degree ; ++d )
         { res = res*cplx ; }
        *itr_rs = res.real() ;
        *itr_is = res.imag() ;
       }
     }
    auto reduce() const
     {
      Complex<fp_t> acc = make_unit_complex<fp_t>(1.,0.) ;
      for ( auto itr_rs = std::begin(m_rs), itr_is = std::begin(m_is) ; itr_rs != std::end(m_rs) ; ++itr_rs, ++itr_is )
       {  acc *= Complex<fp_t>{*itr_rs,*itr_is} ; }
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
