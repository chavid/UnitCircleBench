#include <iostream>
#include <vector>
#include <memory>
#include <cstdlib>
#include <cassert>

struct XY
 {
  double x, y = 0. ;
  virtual void saxpy( double a )
   { y += x*a ; }
 } ;

using XYs = std::vector<std::unique_ptr<XY>> ;

void randomize_x( XYs & xys, int size )
 {
  srand(1) ;
  for ( int i=0 ; i<size ; ++i )
   {
    xys.emplace_back(std::make_unique<XY>()) ;
    xys[i]->x = std::rand()/(RAND_MAX+1.)-0.5 ;
   }
 }

void saxpy( XYs & xys, double a )
 {
  for ( auto & xy : xys )
   { xy->saxpy(a) ; }
 }

double accumulate_y( XYs const & xys, double res )
 {
  for ( auto const & xy : xys )
   { res += xy->y ; }
  return res ;
 }

int main( int argc, char * argv[] )
 {
  assert(argc==3) ;
  int size = atoi(argv[1]) ;
  int repeat = atoi(argv[2]) ;

  std::vector<std::unique_ptr<XY>> xys ;
  randomize_x(xys,size) ;
  while (repeat--)
   { saxpy(xys,0.1) ; }
  double res = accumulate_y(xys,0.)/size ;
    
  std::cout.precision(18) ;
  std::cout<<res<<std::endl ;
 }