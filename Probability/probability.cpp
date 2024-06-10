#include <iostream>
#include <iomanip>
#include <cmath>
#include <string>
#include <vector>

// A framework for testing approximation of double numbers  :
// - Test is the common abtract base class for any test.
// - appromixate_all() run a collection of tests.

class Test
 {
  public :
    Test( std::string const & title ) : title_(title) {}
    std::string const & title() const { return title_ ; }
    virtual double approximate( double ) const {}
    ~Test() {}
  private :
    std::string title_ ;
 } ;

void approximate_all( std::vector<Test const *> & tests, double value )
 {
  std::cout<<std::fixed<<std::left<<std::setprecision(18) ;
  std::vector<Test const *>::iterator itr ;
  for ( itr = tests.begin() ; itr != tests.end() ; ++itr )
   {
    double approx = (*itr)->approximate(value) ;
    std::cout
      << (*itr)->title()
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
    Probability( double value ) ;
    operator double() const ;
  private :
    U numerator_ ;
    unsigned char exponent_ ;
    friend std::ostream & operator<<( std::ostream & os, Probability const & c )
     { return (os<<c.numerator_<<"/2^"<<c.exponent_) ; }
 } ;

template< typename U >
Probability<U>::operator double() const
 { return numerator_/std::pow(2,exponent_) ; }
 
template< typename U >
Probability<U>::Probability( double value )
 {
  U numerator_thread = U(-1)>>1 ;
  double num = value ;
  numerator_ = std::round(num) ;
  exponent_ = 0 ;
  while ((double(*this)!=value)&&(numerator_<numerator_thread))
   {
    exponent_ = exponent_ + 1 ;
    num = num * 2 ;
    numerator_ = std::round(num) ;
   }
 }

// Probability test : approximate a given double with a Probability<U>,
// then get and return the approximated double.

template< typename U >
class ProbabilityTest : public Test
 {
  public :
    ProbabilityTest( std::string const & title ) : Test(title) {}
    double approximate( double value ) const
     { return Probability<U>(value) ; }
 } ;
 
// Main program :
// - prepare tests for the unsigned integral
//   types short, int and long,
// - run the tests for predefined values,
// - delete the tests.

int main()
 {
  std::vector<Test const *> tests ;
  tests.push_back(new ProbabilityTest<unsigned short>("ushort")) ;
  tests.push_back(new ProbabilityTest<unsigned int>("uint  ")) ;
  tests.push_back(new ProbabilityTest<unsigned long>("ulong ")) ;

  std::cout<<std::endl ;
  approximate_all(tests,0.3) ;
  std::cout<<std::endl ;
  approximate_all(tests,1./19068840) ;
  std::cout<<std::endl ;
  approximate_all(tests,1e-400) ;
  std::cout<<std::endl ;

  std::vector<Test const *>::iterator itr ;
  for ( itr = tests.begin() ; itr != tests.end() ; ++itr )
   { delete (*itr) ; }
 }


