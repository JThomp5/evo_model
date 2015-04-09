#ifndef RPI_VERTEX
#define RPI_VERTEX

#include <set>
#include "../../Libraries/Files/StringEx.h"

#include <tr1/memory>

using namespace std;

/**
 *@class Vertex
 *
 *  The Vertex class represents a node in a social network. The (optional) energy variable is used 
 *      construct a degree distribution. The class is designed to work for two tasks:
 *           (a) Generating a synthetic social network model following framework of [insert paper here].
 *           (b) Robustness testing used anonymization of vertex labels.
 *
 *      Extensions or redesigns to the class may need to be made for other uses.
 */

class Vertex { 
 public:
  /**
   *@fn Vertex(unsigned int label):id_(label)
   *@fn Vertex(unsigned int label, double new_energy ):id_(label), new_energy(energy_)
   *
   *Initializes member variables to given values
   *
   *@param label Identification of the vertex
   *@param new_energy Energy value for vertex
   */
 Vertex(unsigned int label):id_(label), energy_(0){};
 Vertex(unsigned int label, double new_energy ):id_(label), energy_(new_energy){};

  /**
   *@fn Vertex ( const Vertex& other )
   *@fn Vertex ( const Vertex* other )
   *
   *Provides a deep copy of given vertex
   * (Right now, this is the same as a shallow copy, but 
   *     should provide a deep copy if this were to change)
   *
   *@param other Vertex to copy
   */
  Vertex ( const Vertex& other ):id_(other.id_), energy_(other.energy_){};
  Vertex ( const Vertex* other ):id_(other->id_), energy_(other->energy_){};
  
  /**
   *@fn Vertex& operator=(const Vertex& other)
   *
   * Assignment. Should provide a deep copy if there is ever a need.
   *
   *@param other Vertex to copy from
   *@return A copy of this object
   */
  Vertex& operator=(const Vertex& other);

  /**
   *@fn bool operator<(const Vertex& other)
   *
   *Compares two vertices ( mainly for use in sets and maps )
   *
   *@return True if this vertex's id_ is less than the others
   */
  bool operator<(const Vertex& other) { return id_ < other.id_; };
  bool operator<(const Vertex& other) const { return id_ < other.id_; };

  /**
   *@fn unsigned int getID ( )
   *@fn double getEnergy ()
   *
   *Simply returns variable values. ('get' methods)  
   *
   *@return The vertex ID and energy value, respectively
   */
  unsigned int getID ( ) { return id_; }
  double getEnergy () { return energy_; }
  double getEnergy () const { return energy_; }

  /**
   *@fn string toString()
   *
   * Retrieves a string representation of the vertex ( just the id )
   *
   *@return String representation of the vertex id
   */
  string toString() {
    return to_str < unsigned int > ( id_ );
  }

  string toString() const {
    return to_str < unsigned int > ( id_ );
  }
  
  /**
   *@fn ~Vertex()
   *
   * Superfluous destructor
   */
  ~Vertex(){};

 private:
  unsigned int id_;
  double energy_;
};

struct cmp_vptr {
  bool operator () ( const shared_ptr < Vertex >& A, const shared_ptr < Vertex >& B ){
    return ( *A < *B );
  }
};

typedef set < shared_ptr < Vertex >, cmp_vptr > vset;

#endif
