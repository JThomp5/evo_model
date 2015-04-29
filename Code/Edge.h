/**
 *@file Edge.h
 *
 *  Declarations for the Edge class, custom comparator for 
 *shared_ptr to Edges, and a helpful typedef for a set of Edges
 *
 *@author James Thompson
 *
 * Copyright James Thompson 2015
 * This program is distributed under the terms of the GNU General Public License
 
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

#ifndef RPI_EDGE
#define RPI_EDGE

#include "Group.h"
#include "../../Libraries/Params/Parameters.h"
#include "../../Libraries/Random/PowerLaw.h"

using namespace std;

/**
 *@class Edge:public Group
 *
 *    Represents an interaction group in the network. Does not technically have
 *  to be between two entities - can be more. Interactions in social networks 
 *  have been found to follow bursty behavior, so to model this, each edge 
 *  tracks the wait-time for the next interaction between entities involved.
 *  The wait times, when modeled with a power law, then result in bursty 
 *  behavior in general. 
 * 
 *    Wait times should be scaled so that one time window equals 1 unit of 
 *  waiting. Then, weight of the edge is the number of interactions that 
 *  happen between time i and time i+1.
 */
class Edge:public Group {
 public:  
  /**
   *@fn Edge()
   *
   * Initializes the next weight time and current edge weight to zero.
   */
 Edge():Group(), wait_time_(0), edge_weight_(0){}
  /**
   *@fn ~Edge()
   */
  ~Edge(){}
  
  /**
   *@fn bool generateWeight(unique_ptr < Parameters >& P )
   *
   *  Simulates waiting times for the edge until the threshold for the current
   * window is reached. The number of interactions that occured is set as the
   * edge weight.
   *
   *@return True if at least one interaction occured in the time window
   */
  bool generateWeight(unique_ptr < Parameters >& P );

  /**
   *@fn string toString()
   *
   *   Constructs a string representation of the edge or interaction.
   *
   *@return A string with multiple lines. Each line corresponds to a pair of
   *        members in the edge. Each line has the format 
   *            'member_a|member_b|edge_weight'
   */
  string toString();

  /**
   *@fn double getWeight ( )
   *
   *  Accesses current edge weight.
   *
   *@return Current count of number of interactions.
   */
  double getWeight ( ) { return edge_weight_; }

 private:
  double wait_time_;         //Tracks time until the next interaction
  double edge_weight_;       //Holds the number of interactions that happened
                             //   in 'current' time window.

  /**
   *@fn double getTotalEnergy( double gravity, double minlag, double max_energy)
   *
   *  Wait times for the edge are modelled by a power law. The power law needs 
   *     a parameter to determine it's behavior ( the exponent ). This function
   *     calculates an appropriate function for the exponent based on the energy
   *     levels of the consituent members.
   *
   *@return A weighted average of the energy levels of edge members
   */
  double getTotalEnergy( double gravity, double minlag, double max_energy);
};

/**
 *@struct cmp_pedge
 *
 *  Custom comparator class for containers of shared_ptrs to Edges
 */
struct cmp_pedge{
  /**
   *@fn bool operator() ( const shared_ptr < Edge >& A, const shared_ptr < Edge >& B )
   *
   * Called for comparison in containers of shared_ptrs to Edges. Simple call 
   *     through to the overloaded comparator for the objects pointed to.
   *
   *@param A First edge to compare
   *@param B Second edge to compare
   *@return True if edge A comes before edge B in the ordering defined by 
   *          Edge::operator<()
   */
  bool operator() ( const shared_ptr < Edge >& A, const shared_ptr < Edge >& B ){
    return ( *A < *B );
  }
};

typedef set < shared_ptr < Edge >, cmp_pedge > eset;

#endif
