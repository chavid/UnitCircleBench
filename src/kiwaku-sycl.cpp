
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
      auto cs0{*this} ;
      auto cv0 = kwk::view{ kwk::source = cs0.data(), kwk::of_size(cs0.size()) } ;
      auto cv = kwk::view{ kwk::source = this->data(), kwk::of_size(this->size()) } ;
      long long repeat = degree - 1 ;
      kwk::transform(m_context,[repeat](auto c, auto c0)
       {
        for ( long long r=0 ; r<repeat ; ++r )
         { c*= c0 ; }
        return c ;
       },cv,cv,cv0) ;
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

      std::vector<FloatingPoint> tmp(m_size), rs0{m_rs}, is0{m_is} ;
      auto tmpv = kwk::view{ kwk::source = tmp.data(), kwk::of_size(m_size) } ;
      auto xrv = kwk::view{ kwk::source = m_rs.data(), kwk::of_size(m_size) } ;
      auto xiv = kwk::view{ kwk::source = m_is.data(), kwk::of_size(m_size) } ;
      auto xrv0 = kwk::view{ kwk::source = rs0.data(), kwk::of_size(m_size) } ;
      auto xiv0 = kwk::view{ kwk::source = is0.data(), kwk::of_size(m_size) } ;
      
       {
      auto tmpp = m_context.inout(tmpv);
      auto xrp  = m_context.inout(xrv);
      auto xip  = m_context.inout(xiv);
      auto xrp0 = m_context.in(xrv0);
      auto xip0 = m_context.in(xiv0);
    
      long long repeat = degree-1 ;
      for ( long long r = 0 ; r <repeat ; ++r )
       {
        kwk::transform_proxy(m_context,[](auto xr, auto xi, auto xr0, auto xi0)
         { return xr*xr0 - xi*xi0 ; }
         ,tmpp,xrp,xip,xrp0,xip0) ;
        kwk::transform_inplace_proxy(m_context,[](auto xi, auto xr, auto xr0, auto xi0)
         { return xi*xr0 + xr*xi0 ; }
         ,xip,xrp,xrp0,xip0) ;
        kwk::copy_proxy(m_context,xrp,tmpp) ;
       }

       }

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

template< template <typename, typename> typename Container, typename FloatingPoint >
void main_final
 ( std::size_t size, long long degree,
   auto & selector )
 {
  kwk::sycl::context context{selector} ;
  Container<decltype(context),FloatingPoint> cs(context,size) ;
  cs.pow(degree) ;
  auto res = cs.reduce() ;
  if (std::abs(1.-res.magnitude())<0.01)
   { std::cout<<myformat("(checksum: {})",res.argument())<<std::endl ; }
  else
   { std::cout<<myformat("(checksum: wrong magnitude {})",res.argument())<<std::endl ; }
 }

template< template <typename, typename> typename Container >
void main_fp
 ( std::string const & fp_name,
   std::size_t size, long long degree,
   auto & selector )
 {
  if (fp_name=="float")
   { main_final<Container,float>(size,degree,selector) ; }
  else if (fp_name=="double")
   { main_final<Container,double>(size,degree,selector) ; }
  else throw "unknown floating point name" ;
 }

void main_arr
 ( std::string const & arr_name,
   std::string const & fp_name,
   std::size_t size, long long degree,
   auto & selector )
 {
  if (arr_name=="aos")
   { main_fp<ComplexesAoS>(fp_name,size,degree,selector) ; }
  else if (arr_name=="soa")
   { main_fp<ComplexesSoA>(fp_name,size,degree,selector) ; }
  else throw "unknown arrangement name" ;
 }

void main_dt
 ( std::string const & dt_name,
   std::string const & arr_name,
   std::string const & fp_name,
   std::size_t size, long long degree )
 {
  if (dt_name=="cpu")
   { main_arr(arr_name,fp_name,size,degree,::sycl::cpu_selector_v) ; }
  else if (dt_name=="gpu")
   { main_arr(arr_name,fp_name,size,degree,::sycl::gpu_selector_v) ; }
  else throw "unknown device type name" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==6) ;
  std::string device_type_name(argv[1]) ;
  std::string arrangement_name(argv[2]) ;
  std::string fp_name(argv[3]) ;
  std::size_t size = {std::strtoull(argv[4],nullptr,10)} ;
  long long degree = {std::strtoll(argv[5],nullptr,10)} ;
  srand(1) ;
  main_dt(device_type_name,arrangement_name,fp_name,size,degree) ;
 }
