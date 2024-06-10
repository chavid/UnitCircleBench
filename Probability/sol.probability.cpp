#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdint>
#include <string_view>
#include <initializer_list>
#include <memory>
#include <type_traits>

// A framework for testing approximation of double numbers  :
// - Test is the common abtract base class for any test.
// - appromixate_all() run a collection of tests.

class Test
 {
  public :
    explicit Test( std::string_view const & title ) : title_{title} {}
    std::string_view const & title() const { return title_ ; }
    virtual double approximate( double ) const = 0 ;
    double approximate( float ) const = delete ;
    double approximate( long double ) const = delete ;
    Test( Test const & ) = delete ;
    Test & operator=( Test const & ) = delete ;
    virtual ~Test() = default ;
  private :
    std::string_view title_ ;
 } ;

using TestPtr = std::unique_ptr<const Test> ;

template <typename TestDerived, typename... Args>
auto make_ptr( Args... args )
 {
  static_assert(std::is_base_of_v<Test,TestDerived>,"Type parameter must inherit from Test.") ;
  return TestPtr{new TestDerived(args...)} ;
 }  

using TestCollection = std::initializer_list<TestPtr> ;

void approximate_all( TestCollection const & tests, double value )
 {
  std::cout<<std::fixed<<std::left<<std::setprecision(18) ;
  for ( auto const & test : tests )
   {
    double approx = test->approximate(value) ;
    std::cout
      << test->title()
      << " : " << std::setw(20) << value
      << " ~ " << std::setw(20) << approx
      << " ("<< std::showpos <<(value-approx) << std::noshowpos << ")"
      << std::endl ;
   }
 }

// Probability class : approximate a double value in the
// interval [0,1] with an unsigned integer divided
// by a power of 2.

template< typename U >
class Probability
 {
  public :  
    explicit Probability( double value ) ;
    explicit operator double() const ;
  private :
    static_assert(std::is_unsigned_v<U>,"Underlying type is not an unsigned integer") ;
    U numerator_ = 0 ;
    unsigned char exponent_ = 0 ;
    friend std::ostream & operator<<( std::ostream & os, Probability const & c )
     { return (os<<c.numerator_<<"/2^"<<c.exponent_) ; }
 } ;

template< typename U >
Probability<U>::operator double() const
 { return numerator_/std::pow(2.,exponent_) ; }
 
template< typename U >
Probability<U>::Probability( double value )
 {
  constexpr auto numerator_thread = static_cast<decltype(numerator_)>(-1)>>1 ;
  auto num = value ;
  numerator_ = std::round(num) ;
  while ((static_cast<decltype(value)>(*this)!=value)&&(numerator_<numerator_thread))
   {
    exponent_ = exponent_ + 1 ;
    num = num * 2 ;
    numerator_ = std::round(num) ;
   }
 }

// Probability class : approximate a double value in the
// interval [0,1] with an unsigned integer divided
// by a power of 2.

template< typename U >
class ProbabilityTest final : public Test
 {
  public :
    using Test::Test ;
    double approximate( double value ) const override
     { return static_cast<decltype(value)>(Probability<U>{value}) ; }
  private :
    static_assert(std::is_unsigned_v<U>,"Underlying type is not an unsigned integer") ;
 } ;
 
// Main program :
// - prepare tests for the unsigned integral
//   types short, int and long,
// - run the tests for 0.1 and pi/4,
// - delete the tests.

int main()
 {
  TestCollection tests =
   {
    make_ptr<ProbabilityTest<uint8_t>> ("uint8 "),
    make_ptr<ProbabilityTest<uint16_t>>("uint16"),
    make_ptr<ProbabilityTest<uint32_t>>("uint32"),
    make_ptr<ProbabilityTest<uint64_t>>("uint64")
   } ;

  std::cout<<std::endl ;
  approximate_all(tests,0.3) ;
  std::cout<<std::endl ;
  approximate_all(tests,1./19068840) ;
  std::cout<<std::endl ;
 }


