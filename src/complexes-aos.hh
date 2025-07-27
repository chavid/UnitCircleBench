

#pragma once

#include "utilities.hh"
#include "unit-complex.hh"

#include <valarray>
#include <vector>
#include <list>
#include <array>


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


