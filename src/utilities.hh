

#pragma once

#include <chrono>
#include <string_view>
#include <iostream>
#include <algorithm>


//================================================================
// Time any function returning void
//================================================================

template< typename Function, typename... ArgTypes >
void time( std::string_view title, Function f, ArgTypes&&... args )
 {
  using namespace std::chrono ;
  auto t1 {steady_clock::now()} ;

  f(std::forward<ArgTypes>(args)...) ;
   
  auto t2 {steady_clock::now()} ;
  auto dt {duration_cast<microseconds>(t2-t1).count()} ;
  std::cout<<"("<<title<<" time: "<<dt<<" us)"<<std::endl ;
 }
 

//================================================================
// Home-made array which is stored in the heap
//================================================================

template< typename T >
class DynArray
 {
  public :
    using value_type = T ;
    explicit DynArray( std::size_t size ) : size_(size), data_(new T [size]) {}
    DynArray( DynArray const & other ) : size_(other.size_), data_(new T [other.size_])
     { std::copy(other.data_,other.data_+size_,data_) ; }
    DynArray( DynArray && other ) : size_(other.size_), data_(other.data_)
     { other.data_ = nullptr ; }
    DynArray & operator=( DynArray const & other )
     {
      if (this==&other) return *this ;
      size_ = other.size_ ;
      data_ = new T [other.size_] ;
      std::copy(other.data_,other.data_+size_,data_) ;
     }
    DynArray & operator=( DynArray && other )
     {
      size_ = other.size_ ;
      data_ = other.data_ ;
      other.data_ = nullptr ;
     }
    T * data() { return data_ ; }
    T * begin() { return data_ ; }
    T * end() { return data_+size_ ; }
    T const * data() const { return data_ ; }
    T const * begin() const { return data_ ; }
    T const * end() const { return data_+size_ ; }
    std::size_t size() const { return size_ ; }
    T & operator[]( std::size_t indice ) { return data_[indice] ; }
    T const & operator[]( std::size_t indice ) const { return data_[indice] ; }
    ~DynArray() { delete [] data_ ; }
  private :
    std::size_t size_ ;
    T * data_ ;
 } ;


//================================================================
// Maximum static size of std::array
//================================================================

constexpr std::size_t MAX_ARRAY_SIZE = 65536 ;


//================================================================
// Etc
//================================================================

template < typename T > class TypeDisplayer ;

