#ifndef RPI_NETWORK
#define RPI_NETWORK

#include "Vertex.h"
#include <set>

using namespace std;

template < typename T >
class Network{
 public:
  Network();
  ~Network();

 private:
  set < Vertex < T > > V;
}

#endif
