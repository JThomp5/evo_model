#include "Vertex.h"

template < typename T >
Vertex<T>& Vertex<T>::operator=(const Vertex& other){
  id_ = other.id_;
  energy_ = other.energy_;

  return *this;
}
