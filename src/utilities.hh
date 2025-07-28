

#pragma once

#include <chrono>
#include <string_view>
#include <iostream>
#include <algorithm>


//================================================================
// Time any function returning void
//================================================================

auto time_before()
 { return std::chrono::steady_clock::now() ; }

void time_after( auto before, std::string_view title )
 {
  using namespace std::chrono ;
  auto after {steady_clock::now()} ;
  auto dt {duration_cast<microseconds>(after-before).count()} ;
  std::cout<<"("<<title<<" time: "<<dt<<" us)"<<std::endl ;
 }

template< typename Function, typename... ArgTypes >
void time( std::string_view title, Function f, ArgTypes&&... args )
 {
  auto before {time_before()} ;
  f(std::forward<ArgTypes>(args)...) ;
  time_after(before,title) ;
 }
 

//================================================================
// Maximum static size of std::array
//================================================================

constexpr std::size_t MAX_ARRAY_SIZE = 65536 ;


//================================================================
// Etc
//================================================================

template < typename T > class TypeDisplayer ;

