#ifndef RPI_VERTEX
#define RPI_VERTEX

/**
 *@class Vertex
 *
 *  The Vertex class represents a node in a social network. The (optional) energy variable is used 
 *      construct a degree distribution. The class is designed to work for two tasks:
 *           (a) Generating a synthetic social network model following framework of [insert paper here].
 *           (b) Robustness testing used anonymization of vertex labels.
 *
 *      Extensions or redesigns to the class may need to be made for other uses.
 *
 *  The typename T clarifies the type of label used for the vertex. Usually it is an int or a string.
 */

template < typename T >
class Vertex { 
 public:
  /**
   *@fn Vertex(T label):id_(label)
   *@fn Vertex(T label, double new_energy ):id_(label), new_energy(energy_)
   *
   *Initializes member variables to given values
   *
   *@param label Identification of the vertex
   *@param new_energy Energy value for vertex
   */
 Vertex(T label):id_(label), energy_(0){};
 Vertex(T label, double new_energy ):id_(label), energy_(new_energy){};

  /**
   *@fn Vertex ( const Vertex& other )
   *
   *Provides a deep copy of given vertex
   * (Right now, this is the same as a shallow copy, but 
   *     should provide a deep copy if this were to change)
   *
   *@param other Vertex to copy
   */
  Vertex ( const Vertex& other ):id_(other.id_), energy_(other.energy_);
  
  /**
   *@fn Vertex& operator=(const Vertex& other)
   *
   */
  Vertex& operator=(const Vertex& other);

  /**
   *@fn boolean operator<(const Vertex& other)
   *
   */
  boolean operator<(const Vertex& other);
  
  

  /**
   *@fn T getID ( )
   *@fn double getEnergy ()
   *
   *Simply returns variable values. ('get' methods)
   *
   *@return The vertex ID and energy value, respectively
   */
  T getID ( ) { return id_; }
  double getEnergy () { return energy_; }

  /**
   *@fn ~Vertex()
   *
   * Superfluous destructor
   */
  ~Vertex(){};

 private:
  T id_;
  double energy_;
}

#endif
