

#pragma once

#include "utilities.hh"
#include "unit-complex.hh"

#include <valarray>
#include <vector>
#include <list>
#include <array>


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

