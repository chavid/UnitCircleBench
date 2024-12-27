
#include <iostream>
#include <cmath>

int main()
 {
  float r = std::cos(2.71827), new_r ;
  float i = std::sin(2.71827), new_i ;
  for ( std::size_t repeat = 0 ; repeat < 16 ; ++repeat )
   {
    new_r = r*r - i*i ;
    new_i = r*i + r*i ;
    r = new_r ;
    i = new_i ;
   }
  std::cout<<r<<" "<<i<<std::endl ;
 }
 
