template< typename T >
class DynArray
 {
  public :
    explicit DynArray( int size ) : size_(size), data_(new T [size]) {}
    T * begin() { return data_ ; }
    T * end() { return data_+size_ ; }
    int size() { return size_ ; }
    T & operator[]( int indice ) { return data_[indice] ; }
    T const & operator[]( int indice ) const { return data_[indice] ; }
    ~DynArray() { delete [] data_ ; }
  private :
    int size_ ;
    T * data_ ;
 } ;

