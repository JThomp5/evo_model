#include "Vertex.h"

/**
 *@fn Vertex& Vertex::operator=(const Vertex& other)
 *
 *Simply copies over all members when assignment occurs. If
 *   the need ever arises, this should produce a deep copy.
 *
 *@param other Vertex to copy
 *@return Reference to this object ( for chaining )
 */
Vertex& Vertex::operator=(const Vertex& other){
  id_ = other.id_;
  energy_ = other.energy_;

  return *this;
}
