/**
 *@file Vertex.h
 *
 *Definitions for the Vertex class
 *
 *@author James Thompson
 *
 *Copyright James Thompson 2015
 *This program is distributed under the terms of the GNU General Public License

This file is part of RPI-evo-model.                                                                                                   
    RPI-evo-model is free software: you can redistribute it and/or modify                                                              
    it under the terms of the GNU General Public License as published by                                                               
    the Free Software Foundation, either version 3 of the License, or                                                                  
    (at your option) any later version.                                                                                               

    RPI-evo-model is distributed in the hope that it will be useful,                                                                   
    but WITHOUT ANY WARRANTY; without even the implied warranty of                                                               
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                                                                   
    GNU General Public License for more details.                                                                                      

    You should have received a copy of the GNU General Public License                                                                  
    along with RPI-evo-model.  If not, see <http://www.gnu.org/licenses/>.
 */

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
 Vertex(unsigned int label):id_(label), energy_(0), edge_count_(0){};
 Vertex(unsigned int label, double new_energy ):id_(label), energy_(new_energy), edge_count_(0){};

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
   * Assignment. 
   * Simple copy of members at this point.
   * Should provide a deep copy if there is ever a need.
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
   *@fn double getEnergy () const
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
   *@fn string toString() const
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

  
  void resetEdgeCount() { edge_count_ = 0; }
  void incrementEdgeCount(){ ++edge_count_; }

  /**
   *@fn unsigned int getEdgeCount()
   *
   * Simply retreives the edge_count_ value.
   *    edge_count_ probably could have been public, to be honest
   * 
   *@return Number of active edges this vertex is involved in
   *            (active means there's a positive weight)
   */
  unsigned int getEdgeCount() { return edge_count_; }

  /**
   *@fn ~Vertex()
   *
   * Superfluous destructor
   */
  ~Vertex(){};

 private:
  unsigned int id_;              //Identifier
  double energy_;                //Energy value for hub determination
  unsigned int edge_count_;      //Number of active edges vertex
                                 //is part of
};

/**
 *@fn cmp_vptr 
 *
 * Custom comparator for shared_ptr < Vertex > containers
 * Simply calls through to the overloaded operator<()
 */
struct cmp_vptr {
  bool operator () ( const shared_ptr < Vertex >& A, const shared_ptr < Vertex >& B ){
    return ( *A < *B );
  }
};

typedef set < shared_ptr < Vertex >, cmp_vptr > vset;

#endif
