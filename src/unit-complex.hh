
#pragma once

#include <concepts> // std::floating_point
#include <cstdlib>  // rand
#include <numbers>  // pi
#include <cmath>    // cos, sin, sqrt, atan


//================================================================
// Minimal complex class.
//================================================================

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

    Complex & operator*=( Complex const & other )
     { return operator=((*this)*other) ; }
    friend Complex operator*( Complex const & lhs, Complex const & rhs )
     {
      auto r = lhs.m_r*rhs.m_r - lhs.m_i*rhs.m_i ;
      auto i = rhs.m_r*lhs.m_i + lhs.m_r*rhs.m_i ;
      return {r,i} ;
    }

  private :

    fp_t m_r {}, m_i {} ;

 } ;


//================================================================
// Utility functions for the creation of unit complexes.
//================================================================

// help using std::float16_t adn std::float128_t
void cast_and_store( std::floating_point auto & var, std::floating_point auto val  )
 {
  using fp_type = std::remove_reference_t<decltype(var)> ;
  var = static_cast<fp_type>(val) ;
 }

void random_unit_complex( std::floating_point auto & r, std::floating_point auto & i  )
 {
  auto e = std::rand() ;
  cast_and_store(r,std::cos(e)) ;
  cast_and_store(i,std::sin(e)) ;
 }

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

