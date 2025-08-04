
#include "utilities.hh"
#include "unit-complex.hh"
#include "dyn-array.hh"

#include <kwk/container.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/algorithm/algos/copy.hpp>

#include <kwk/context/cpu/context.hpp>
#ifdef __INTEL_LLVM_COMPILER
#  include <kwk/context/sycl/context.hpp>
#endif

#include <cassert> // for assert
#include <vector>


//================================================================
// AoS
//================================================================

template< typename Context, std::floating_point FloatingPoint >
class ComplexesAoS : public std::vector<Complex<FloatingPoint>>
 {
  public :
    ComplexesAoS
     ( Context & context, std::size_t a_size )
     : std::vector<Complex<FloatingPoint>>(a_size), m_context(context)
     {
      for ( auto & elem : *this )
       { elem = make_random_unit_complex<FloatingPoint>() ; }
     }
    void pow
     ( long long degree )
     {
      auto before = time_before() ;
      auto cs0{*this} ;
      auto cv0 = kwk::view{ kwk::source = cs0.data(), kwk::of_size(cs0.size()) } ;
      auto cv = kwk::view{ kwk::source = this->data(), kwk::of_size(this->size()) } ;
      long long repeat = degree - 1 ;
      kwk::transform(m_context,[repeat](auto c, auto c0)
       {
        for ( long long r=0 ; r<repeat ; ++r )
         { c*= c0 ;  }
        return c ;
       },cv,cv,cv0) ;
      time_after(before,"pow") ;
     }
    Complex<FloatingPoint> reduce() const
     {
      auto res  = make_unit_complex<FloatingPoint>(1.,0.) ;
      for ( auto const & elem : *this )
       { res *= elem ; }
      return res ;
     }
   private :
     Context & m_context ;
 } ;


//================================================================
// SoA of complex numbers
//================================================================

template< typename Context, std::floating_point FloatingPoint >
class ComplexesSoA
 {
  public :  
    ComplexesSoA( Context & context, std::size_t size ) : m_context(context), m_size(size), m_rs(size), m_is(size)
     {
      for ( std::size_t i = 0 ; i < m_size ; ++i )
       { random_unit_complex(m_rs[i],m_is[i]) ; }
     }  
    void pow( long long degree )
     {
      auto before = time_before() ;
      std::vector<FloatingPoint> tmp(m_size), rs0{m_rs}, is0{m_is} ;
      auto tmpv = kwk::view{ kwk::source = tmp.data(), kwk::of_size(m_size) } ;
      auto xrv = kwk::view{ kwk::source = m_rs.data(), kwk::of_size(m_size) } ;
      auto xiv = kwk::view{ kwk::source = m_is.data(), kwk::of_size(m_size) } ;
      auto xrv0 = kwk::view{ kwk::source = rs0.data(), kwk::of_size(m_size) } ;
      auto xiv0 = kwk::view{ kwk::source = is0.data(), kwk::of_size(m_size) } ;
      long long repeat = degree-1 ;
      for ( long long r = 0 ; r <repeat ; ++r )
       {
        kwk::transform(m_context,[](auto xr, auto xi, auto xr0, auto xi0)
         { return xr*xr0 - xi*xi0 ; }
         ,tmpv,xrv,xiv,xrv0,xiv0) ;
        kwk::transform(m_context,[](auto xr, auto xi, auto xr0, auto xi0)
         { return xi*xr0 + xr*xi0 ; }
         ,xiv,xrv,xiv,xrv0,xiv0) ;
        kwk::copy(m_context,xrv,tmpv) ;
       }
      time_after(before,"pow") ;
    }
    auto reduce() const
     {
      Complex<FloatingPoint> acc = make_unit_complex<FloatingPoint>(1.,0.) ;
      for ( std::size_t i = 0 ; i < m_size ; ++i )
       { acc *= Complex<FloatingPoint>{m_rs[i],m_is[i]} ; }
      return acc ;
     }    
  private :
    Context & m_context ;
    std::size_t m_size ;
    std::vector<FloatingPoint> m_rs, m_is ;
 } ;


//================================================================
// Main
//================================================================

template< typename Collection >
void main_impl( Collection & collection, long long degree )
 {
  collection.pow(degree) ;
  auto res = collection.reduce() ;
  if (std::abs(1.-res.magnitude())<0.01)
   { std::cout<<myformat("(checksum: {})",res.argument())<<std::endl ; }
  else
   { std::cout<<myformat("(checksum: wrong magnitude {})",res.argument())<<std::endl ; }
 }

template< std::floating_point fp_t >
void main_aos( std::size_t size, long long degree )
 {
  kwk::sycl::context context{::sycl::gpu_selector_v} ;
  ComplexesAoS<decltype(context),fp_t> cs(context,size) ;
  main_impl(cs,degree) ;
 }

template< std::floating_point fp_t >
void main_soa( std::size_t size, long long degree )
 {
  kwk::sycl::context context{::sycl::gpu_selector_v} ;
  ComplexesSoA<decltype(context),fp_t> cs(context,size) ;
  main_impl(cs,degree) ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==5) ;
  std::string arrangement_tname(argv[1]) ;
  std::string fp_tname(argv[2]) ;
  std::size_t size = {std::strtoull(argv[3],nullptr,10)} ;
  long long degree = {std::strtoll(argv[4],nullptr,10)} ;
  srand(1) ;
  
  if (arrangement_tname=="aos")
   {
    if (fp_tname=="float") time("main",main_aos<float>,size,degree) ;
    else if (fp_tname=="double") time("main",main_aos<double>,size,degree) ;
    else throw "unknown precision" ;
   }
  else if (arrangement_tname=="soa")
   {
    if (fp_tname=="float")
     { time("main",main_soa<float>,size,degree) ; }
    else if (fp_tname=="double")
     { time("main",main_soa<double>,size,degree) ; }
    else throw "unknown precision" ;
   }
  else throw "unknown arrangement_tname" ;
 }
