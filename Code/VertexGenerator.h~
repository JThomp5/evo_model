#ifndef RPI_VERTGEN
#define RPI_VERTGEN

#include "Vertex.h"
#include <set>
#include <map>
#include <vector>

using namespace std;

template < typename T >
class VertexGenerator {
 public:
 VertexGenerator( T first, T (*generator)(T base) ):current_(first), gen_(generator){};
  ~VertexGenerator(){};
  
  /**
   *@fn Vertex < T > next()
   *
   *
   */
  Vertex < T > next(){
    current_ = gen_(current_);
    return Vertex < T > ( current_ );
  };
 private:
  T current_;
  T (*gen_)(T base);
};

unsigned int next_int ( unsigned int base);
string next_string ( string base );
string next_string2 ( string base );

#endif
