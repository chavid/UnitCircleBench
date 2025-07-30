
#include "utilities.hh"
#include "unit-complex.hh"
#include "dyn-array.hh"

#include <eve/module/algo.hpp>

#include <cassert> // for assert
#include <vector>


//================================================================
// Utilities
//================================================================

void init_random_soa( auto && reals, auto && imags, std::size_t size )
 {
  for ( std::size_t i = 0 ; i < size ; ++i )
   { random_unit_complex(reals[i],imags[i]) ; }
 }

auto reduce_soa( auto && reals, auto && imags, std::size_t size )
 {
  using array_type = std::remove_reference_t<decltype(reals)> ;
  using fp_type = typename array_type::value_type ;
  Complex<fp_type> acc = make_unit_complex<fp_type>(1.,0.) ;
  for ( std::size_t i = 0 ; i < size ; ++i )
   { acc *= Complex<fp_type>{reals[i],imags[i]} ; }
  return acc ;
 }


//================================================================
// Implem : home-made eve::wide
//================================================================

template< std::floating_point fp_t >
class ComplexesRaw
 {
  public :  
    ComplexesRaw( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { init_random_soa(m_rs,m_is,m_size) ; }
    void pow( long long degree )
     {
      std::vector<fp_t> tmp(m_size), a_rs{m_rs}, a_is{m_is} ;
      auto before = time_before() ;
      for ( long long d = 1 ; d <degree ; ++d )
      for ( std::size_t i = 0 ; i<m_size ; ++i )
       {
        tmp[i] = m_rs[i]*a_rs[i] - m_is[i]*a_is[i] ;
        m_is[i] = m_is[i]*a_rs[i] + m_rs[i]*a_is[i] ;
        m_rs[i] = tmp[i] ;
       }
      time_after(before,"pow") ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_size) ; }    
  private :
    std::size_t m_size ;
    std::vector<fp_t> m_rs, m_is ;
 } ;


//================================================================
// Implem : home-made eve::wide
//================================================================

template< std::floating_point fp_t >
class ComplexesWide
 {
  public :  
    ComplexesWide( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { init_random_soa(m_rs,m_is,m_size) ; }
    void pow( long long degree )
     {
      std::size_t step = eve::cardinal_v<eve::wide<fp_t>> ;
      std::vector<fp_t> a_rs{m_rs}, a_is{m_is} ;
      auto before = time_before() ;
      for ( long long d = 1 ; d <degree ; ++d )
       {
        std::size_t i, size1 = m_size-step ;
        for ( i=0 ; i<=size1 ; i+=step )
         {
          eve::wide<fp_t> arw(&a_rs[i]), aiw(&a_is[i]), mrw(&m_rs[i]), miw(&m_is[i]) ;
          eve::wide<fp_t> tmpw = mrw*arw - miw*aiw ;
          miw = miw*arw + mrw*aiw ;
          eve::store(tmpw,&m_rs[i]) ;
          eve::store(miw,&m_is[i]) ;
         }
        for ( ; i<m_size ; ++i )
         {
          auto tmp = m_rs[i]*a_rs[i] - m_is[i]*a_is[i] ;
          m_is[i] = m_is[i]*a_rs[i] + m_rs[i]*a_is[i] ;
          m_rs[i] = tmp ;
         }
       }
      time_after(before,"pow") ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_size) ; }    
  private :
    std::size_t m_size ;
    std::vector<fp_t> m_rs, m_is ;
 } ;


//================================================================
// Implem : transform_to
//================================================================

template< std::floating_point fp_t >
class ComplexesTransform
 {
  public :  
    ComplexesTransform( std::size_t size ) : m_size(size), m_rs(size), m_is(size)
     { init_random_soa(m_rs,m_is,m_size) ; }
    void pow( long long degree )
     {
      std::vector<fp_t> tmp(m_size), a_rs{m_rs}, a_is{m_is} ;
      auto before = time_before() ;
      for ( long long d = 1 ; d <degree ; ++d )
       {
        auto z = eve::views::zip(a_rs,a_is,m_rs,m_is) ;
        eve::algo::transform_to[eve::algo::no_aligning]
         ( z,tmp,[](auto & v)
           { return get<2>(v)*get<0>(v) - get<3>(v)*get<1>(v) ; }) ;
        eve::algo::transform_to[eve::algo::no_aligning]
         ( z,m_is,[](auto & v)
           { return get<3>(v)*get<0>(v) + get<2>(v)*get<1>(v) ; }) ;
        std::swap(m_rs,tmp) ;
       }
      time_after(before,"pow") ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_size) ; }    
  private :
    std::size_t m_size ;
    std::vector<fp_t> m_rs, m_is ;
 } ;


//================================================================
// Main
//================================================================

template< typename Implem >
void main_impl( std::size_t size, long long degree )
 { 
  Implem collection(size) ;
  collection.pow(degree) ;
  auto res = collection.reduce() ;
  if (std::abs(1.-res.magnitude())<0.01)
   { std::cout<<std::format("(checksum: {})",res.argument())<<std::endl ; }
  else
   { std::cout<<std::format("(checksum: wrong magnitude {})",res.argument())<<std::endl ; }
 }

template< std::floating_point fp_t >
void main_fp( std::string const & implem_tname, std::size_t size, long long degree )
 {
  if (implem_tname=="raw")
   { main_impl<ComplexesRaw<fp_t>>(size,degree) ; }
  else if (implem_tname=="wide")
   { main_impl<ComplexesWide<fp_t>>(size,degree) ; }
  else if (implem_tname=="transform")
   { main_impl<ComplexesTransform<fp_t>>(size,degree) ; }
  else throw "unknown implem_tname" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==5) ;
  std::string implem_tname(argv[1]) ;
  std::string fp_tname(argv[2]) ;
  std::size_t size = {std::strtoull(argv[3],nullptr,10)} ;
  long long degree = {std::strtoll(argv[4],nullptr,10)} ;
  srand(1) ;

  //std::cout << eve::current_api << "\n";

  if (fp_tname=="float")
   { time("main",main_fp<float>,implem_tname,size,degree) ; }
  else if (fp_tname=="double")
   { time("main",main_fp<double>,implem_tname,size,degree) ; }
  else throw "unknown precision" ;
 }
