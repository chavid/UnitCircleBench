
#include "utilities.hh"
#include "unit-complex.hh"
#include "dyn-array.hh"

#include <kwk/container.hpp>
#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/algorithm/algos/copy.hpp>

#include <kwk/context/cpu/context.hpp>
#include <kwk/context/eve/context.hpp>
#ifdef __INTEL_LLVM_COMPILER
#  include <kwk/context/sycl/context.hpp>
#endif

#include <cassert> // for assert
#include <vector>


//================================================================
// AoS
//================================================================

void init_random_aos( auto && collection )
 {
  using collection_type = std::remove_reference_t<decltype(collection)> ;
  using value_type = typename collection_type::value_type ;
  using fp_type = typename value_type::fp_type ;
  for ( auto & elem : collection )
   { elem = make_random_unit_complex<fp_type>() ; }
 }

template< typename Context, kwk::concepts::view V >
void ax_aos
 ( Context & context, V & av, V & xv, long long repeat )
 {
  for ( long long d=0 ; d<repeat ; ++d )
   {
    kwk::transform(context,[](auto a, auto x)
     { return a*x ; },xv,av,xv) ;
   }
 }

auto reduce_aos( auto && collection )
 {
  using collection_type = std::remove_reference_t<decltype(collection)> ;
  using value_type = typename collection_type::value_type ;
  using fp_type = typename value_type::fp_type ;
  value_type res  = make_unit_complex<fp_type>(1.,0.) ;
  for ( auto const & elem : collection )
   { res *= elem ; }
  return res ;
 }

template< typename Context, std::floating_point fp_t >
class ComplexesAoS : public std::vector<Complex<fp_t>>
 {
  public :
    ComplexesAoS( Context & context, std::size_t a_size ) : std::vector<Complex<fp_t>>(a_size), m_context(context)
     { init_random_aos(*this) ; }
    void pow( long long degree )
     {
      std::vector<Complex<fp_t>> as{*this} ;
      auto av = kwk::view{ kwk::source = as.data(), kwk::of_size(as.size()) } ;
      auto xv = kwk::view{ kwk::source = this->data(), kwk::of_size(this->size()) } ;
      ax_aos(m_context,av,xv,degree-1) ;
     }
    Complex<fp_t> reduce() const
     { return reduce_aos(*this) ; }
  private :
    Context & m_context ;
 } ;


//================================================================
// SoA of complex numbers
//================================================================

void init_random_soa( auto && reals, auto && imags, std::size_t size )
 {
  for ( std::size_t i = 0 ; i < size ; ++i )
   { random_unit_complex(reals[i],imags[i]) ; }
 }

template< typename Context, kwk::concepts::view V >
void ax_soa
 ( Context & context, V & tmp, V & areals, V & aimags, V & xreals, V & ximags, long long repeat )
 {
  auto before = time_before() ;
  for ( long long d = 0 ; d <repeat ; ++d )
   {
    kwk::transform(context,[](auto ar, auto ai, auto xr, auto xi)
     { return xr*ar - xi*ai ; }
     ,tmp,areals,aimags,xreals,ximags) ;
    kwk::transform(context,[](auto ar, auto ai, auto xr, auto xi)
     { return xi*ar + xr*ai ; }
     ,ximags,areals,aimags,xreals,ximags) ;
    kwk::copy(context,xreals,tmp) ;
   }
  time_after(before,"pow") ;
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

template< typename Context, std::floating_point fp_t >
class ComplexesSoA
 {
  public :  
    ComplexesSoA( Context & context, std::size_t size ) : m_context(context), m_size(size), m_rs(size), m_is(size)
     { init_random_soa(m_rs,m_is,m_size) ; }
    void pow( long long degree )
     {
      std::vector<fp_t> tmp(m_size), a_rs{m_rs}, a_is{m_is} ;
      auto tmpv = kwk::view{ kwk::source = tmp.data(), kwk::of_size(m_size) } ;
      auto arv = kwk::view{ kwk::source = a_rs.data(), kwk::of_size(m_size) } ;
      auto aiv = kwk::view{ kwk::source = a_is.data(), kwk::of_size(m_size) } ;
      auto xrv = kwk::view{ kwk::source = m_rs.data(), kwk::of_size(m_size) } ;
      auto xiv = kwk::view{ kwk::source = m_is.data(), kwk::of_size(m_size) } ;
      ax_soa(m_context,tmpv,arv,aiv,xrv,xiv,degree-1) ;
     }
    auto reduce() const
     { return reduce_soa(m_rs,m_is,m_size) ; }    
  private :
    Context & m_context ;
    std::size_t m_size ;
    std::vector<fp_t> m_rs, m_is ;
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
void main_aos( std::string execution_tname, std::size_t size, long long degree )
 {
  if (execution_tname=="cpu")
   {
    ComplexesAoS<decltype(kwk::cpu),fp_t> collection(kwk::cpu,size) ;
    main_impl(collection,degree) ;
   }
  else if (execution_tname=="simd")
   {
    ComplexesAoS<decltype(kwk::simd),fp_t> collection(kwk::simd,size) ;
    main_impl(collection,degree) ;
   }
#ifdef __INTEL_LLVM_COMPILER
  else if (execution_tname=="sycl")
   {
    static kwk::sycl::context context{::sycl::gpu_selector_v} ;
    ComplexesAoS<decltype(context),fp_t> collection(context,size) ;
    main_impl(collection,degree) ;
   }
#endif
  else throw "unknown execution_tname" ;
 }

template< std::floating_point fp_t >
void main_soa( std::string execution_tname, std::size_t size, long long degree )
 {
  if (execution_tname=="cpu")
   {
    ComplexesSoA<decltype(kwk::cpu),fp_t> collection(kwk::cpu,size) ;
    main_impl(collection,degree) ;
   }
  else if (execution_tname=="simd")
   {
    ComplexesSoA<decltype(kwk::simd),fp_t> collection(kwk::simd,size) ;
    main_impl(collection,degree) ;
   }
#ifdef __INTEL_LLVM_COMPILER
  else if (execution_tname=="sycl")
   {
    static kwk::sycl::context context{::sycl::gpu_selector_v} ;
    ComplexesSoA<decltype(context),fp_t> collection(context,size) ;
    main_impl(collection,degree) ;
   }
#endif
  else throw "unknown execution_tname" ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==6) ;
  std::string execution_tname(argv[1]) ;
  std::string arrangement_tname(argv[2]) ;
  std::string fp_tname(argv[3]) ;
  std::size_t size = {std::strtoull(argv[4],nullptr,10)} ;
  long long degree = {std::strtoll(argv[5],nullptr,10)} ;
  srand(1) ;
  
  if (arrangement_tname=="aos")
   {
    if (fp_tname=="float") time("main",main_aos<float>,execution_tname,size,degree) ;
    else if (fp_tname=="double") time("main",main_aos<double>,execution_tname,size,degree) ;
    else throw "unknown precision" ;
   }
  else if (arrangement_tname=="soa")
   {
    if (fp_tname=="float")
     { time("main",main_soa<float>,execution_tname,size,degree) ; }
    else if (fp_tname=="double")
     { time("main",main_soa<double>,execution_tname,size,degree) ; }
    else throw "unknown precision" ;
   }
  else throw "unknown arrangement_tname" ;
 }
