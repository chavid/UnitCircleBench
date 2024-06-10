
// This works with any function whose return type is void

#include <chrono>
#include <string_view>
#include <iostream>

template< typename Fonction, typename... ArgTypes >
void time( std::string_view title, Fonction f, ArgTypes&&... args )
 {
  using namespace std::chrono ;
  auto t1 {steady_clock::now()} ;

  f(std::forward<ArgTypes>(args)...) ;
   
  auto t2 {steady_clock::now()} ;
  auto dt {duration_cast<microseconds>(t2-t1).count()} ;
  std::cout<<"("<<title<<" time: "<<dt<<" us)"<<std::endl ;
 }
 