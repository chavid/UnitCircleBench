
//================================================================
// Home-made array which is stored in the heap
//================================================================

#pragma once

#include <cstddef> // std::size_t

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
      delete [] data_ ;
      size_ = other.size_ ;
      data_ = new T [other.size_] ;
      std::copy(other.data_,other.data_+size_,data_) ;
     }
    DynArray & operator=( DynArray && other )
     {
      delete [] data_ ;
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

