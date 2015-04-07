#ifndef RPI_NETWORK
#define RPI_NETWORK

#include "Vertex.h"
#include "VertexGenerator.h"
#include <set>
#include <tr1/memory>
#include <iostream>

using namespace std;

template < typename T >
class Network{
 public:
  /*Network(T first_node, T (*vg) (T base), unsigned int size): gen_(first_node, vg){
    for ( int i = 0; i < size; i++ ){
      V_.insert ( gen_.next() );
    }
    
    printVertices(); 
    };*/

  Network ( unique_ptr < Parameters >& P, VertexGenerator < T > vgen ){
    unsigned int num_vert = P->get < unsigned int > ( "V", 100 );
    for ( unsigned int i = 0; i < num_vert; i++ ){
      V_.insert ( gen_.next() );
    }

    printVertices();
  }

  ~Network(){};

  void printVertices() {
    typename set < Vertex < T > >::iterator it_v;
    
    for ( it_v = V_.begin(); it_v != V_.end(); it_v++ ){
      cout << it_v->toString() << endl;
    }
  };
 private:
  set < Vertex < T > > V_;
  VertexGenerator < T > gen_;
};

#endif
