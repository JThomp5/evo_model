/**
 *@file Network.h
 *
 *Definitions for the Network class
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
    but WITHOUT ANY WARRANTY; without even the implied 
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                         
    GNU General Public License for more details.                                                                                  

    You should have received a copy of the GNU General Public License                                                                  
    along with RPI-evo-model.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef RPI_NETWORK
#define RPI_NETWORK

#include "Vertex.h"
#include "Community.h"
#include "Edge.h"
#include "../../Libraries/Random/PowerLaw.h"
#include "../../Libraries/Random/Wrappers.h"
#include "../../Libraries/Params/Parameters.h"
#include <set>
#include <tr1/memory>
#include <algorithm>
#include <iostream>
#include <fstream>

using namespace std;

/**
 *@class Network
 *
 * Representation of a single window for a temporal network.
 *     To generate a temporal network, this structure is changed
 *     in-line. Holding multiple windows in memory was too much
 *     for my computer to handle. Information about windows needs
 *     to be printed out during the construction process.
 *
 * All vertices are represented by an unsigned integer. Transfer
 *     to and from strings must be done separately.
 *
 * For use in the network generation model and anonymous testing
 *     framework described in [].
 *
 * @author James Thompson
 */
class Network{
 public:
  /**
   *@fn Network ( unique_ptr < Parameters >& P ): next_id_(0), vpl_( new PowerLaw ( -P->get < double > ( "vexp" ), P->get < double > ( "vmin" ), P->get < double > ( "vmax" ) ) )
   *
   * Initializes a certain number of vertices by assigning them an
   *    energy value based off of the given parameters for the
   *    power law. Default power law values result in a NULL object
   *    for use when analyzing exitsing networks (and vpl_ becomes 
   *    unnecessary).
   *
   *@param P See README for description of parameters
   */
  Network ( unique_ptr < Parameters >& P ): total_energy_(0), next_id_(0), vpl_( new PowerLaw ( -P->get < double > ( "vexp", 1.75 ), P->get < double > ( "vmin", 0.4 ), P->get < double > ( "vmax", 1 ) ) ), cpl_( new PowerLaw ( -(P->get < double > ( "cexp", 2.75 ) ), P->get < double > ("cmin", 3), P->get<double>("cmax", 55) ) ), current_window_(0){
    if ( vpl_->getExp() > 0) { 
      cerr << "Setting vertex energy power law to NULL. Set -vexp to change." << endl;
      vpl_ = NULL;
    }
    srand ( time ( NULL ) );
  }

  /**
   *@fn ~Network()
   */
  ~Network(){};

  /**
   *@fn RandomNetwork ( unique_ptr < Parameters >& P )
   *
   *  Main controller for constructing a random network with random
   *communities and edge structure.
   *  
   *@param P Object holding parameters for model ( usually 
   *            from command line arguments )
   */
  void RandomNetwork ( unique_ptr < Parameters >& P );

  /**
   *@fn shared_ptr < Community > RandomCommunity
   *
   *    Constructs a group of random members to consider a 
   * community.
   *
   *@param size Number of members to construct community with
   *@return Pointer to community object with random members
   */
  shared_ptr < Community > RandomCommunity ( unsigned int size );

  /**
   *@fn void printCommunities()
   *
   *    Outputs a representation of the community structure of the
   * network to standard output. Each community's string
   * representation begins on a fresh line.
   */
  void printCommunities(){
    for ( int i = 0; i < C_.size(); i++ ){
      cout << C_[i]->toString() << endl;
    }
  }

  /**
   *@fn void printEdges ()
   *
   *  Prints a representation of all active edges in the network
   * to standard output.
   */
  void printEdges ( ) {
    eset::iterator it_e;
    for ( it_e = E_.begin(); it_e != E_.end(); it_e++ ){
      cout << (*it_e)->toString() << " " << (*it_e)->getWeight() << endl;
    }
  }

  /**
   *@fn void fillCommunities()
   * 
   * Makes sure that each vertex in the network is a member of
   * at least one community by adding random communities using
   * vertices with no memberships until every vertex is covered.
   */
  void fillCommunities( );
  
  /**
   *@fn void populateEdges ( unique_ptr < Parameters >& P )
   *
   *    Considers each pair of vertices that share at least
   * one community, and a specified number of noise edges. Runs
   * a random process to determine the weights on each edge.
   *
   *@param P Parameters for the model ( usually from the command line)
   */
  void populateEdges ( unique_ptr < Parameters >& P );

  /**
   *@fn void printVertices()
   * 
   * Prints the list of vertices in the network and their energy
   *  values (one pair per line )
   *  to standard out. Used for debugging purposes.
   */
  void printVertices() {
    set < shared_ptr < Vertex > >::iterator it_v;
    
    for ( it_v = V_.begin(); it_v != V_.end(); it_v++ ){
      cout << (*it_v)->toString() << " : " << (*it_v)->getEnergy() <<  endl;
    }
  };

  /**
   *@fn unsigned int NumVerts ( )
   * 
   * Calculates the number of vertices currently in the network
   *
   *@return Vertex count of network
   */
  unsigned int NumVerts ( ){
    return V_.size();
  }

  /**
   *void printVerts()
   *
   *  Prints the names of vertices ( one per line ) to standard out
   */
  void printVerts(){
    vset::iterator it_v = V_.begin(); 
    while ( it_v != V_.end() ){
      cout << (*it_v)->toString() << " ";

      ++it_v;
    }
    cout << endl;
  }

  /**
   *@fn void printNetwork ( string filename )
   *
   *   Prints the edge list for the network, one edge string
   * representation on each newline, to the given file.
   *
   *@param filename File to print network to
   */
  void printNetwork ( string filename );

  /**
   *@fn void addRandomVertex ()
   *
   * Adds a vertex to the network with an ID one higher than the 
   *    current maximum. Also generates an energy value for the 
   *    vertex.
   */
  void addRandomVertex ( );
  /**
   * @fn void genNextTimeWindow( unique_ptr < Parameters >& P )
   *
   * Main controller for calling all appropriate functions
   *    for changing network structure between time windows and
   *    embedding evolutions.
   *
   * @param P Parameters for model ( from command line usually )
   */
  void genNextTimeWindow( unique_ptr < Parameters >& P );

  /**
   *@fn shared_ptr < Vertex > getRandomVertex ( )
   *
   *  Selects a random vertex from the network with regards to 
   * energy levels ( higher energy means greater chance of being
   * chosen ). If an error occurs for some reason, the first
   * vertex is returned by default.
   *
   *@return Pointer to a random vertex in the network
   */
  shared_ptr < Vertex > getRandomVertex ( ){
    //Gets random energy
    double energy_seen = 0;
    double random_target = random_double() * total_energy_;
    
    //Iterates through vertices until energy is hit
    //   Vertex at that spot is returned.
    vset::iterator it_v = V_.begin();
    while ( ( energy_seen < random_target ) && (it_v != V_.end()) ){
      energy_seen += (*it_v)->getEnergy();
      if ( energy_seen > random_target ){
	return *it_v;
      }
      ++it_v;
    }
    
    return (*(V_.begin()));
  }

 private:
  set < shared_ptr < Vertex >, cmp_vptr > V_;              //Vertex structure
  vector < shared_ptr < Community > > C_;     //Community structure
  eset E_;                        //Edges of network
  //Map to track which vertices are in which communities
  map < shared_ptr < Vertex >, int, cmp_vptr > membership_; 
  unsigned int next_id_;          //Largest id
  double total_energy_;           //Sum of vertex energies
  unique_ptr < PowerLaw > vpl_;   //Power law for energy values
  unique_ptr < PowerLaw > cpl_;   //Communty sizes
  
  int current_window_;

  /**
   *@fn void addCommunity( shared_ptr < Community > C )
   *
   *  Makes sure that each vertex in the community is mapped
   *back to the community as well as the community tracked
   *for future reference.
   *
   *@param C Community to add to structure for the network
   */
  void addCommunity( shared_ptr < Community > C ) {
    C_.push_back ( C );
    
    const vset c_mem = C->getMembers();
    vset::const_iterator it_c;
    for ( it_c = c_mem.begin(); it_c != c_mem.end(); it_c++ ){
      membership_.insert(pair < shared_ptr < Vertex >, int > ( *it_c, C_.size() - 1 ) );
    }
  }
  
  /**
   *@fn void deathEvents ( double dprob )
   *
   * Considers all communities and randomly deletes (does
   *    not generate the next step of the evolution ) communities
   *    using a given parameter. 
   *
   * Easy extension: make probability of death depend on 
   *    community features.
   *
   *@param dprob Probability for a community to undergo a death
   *             event in any time window.
   */
  void deathEvents ( double dprob );

  /**
   *@fn void birthEvents ( double bprop )
   *
   *Constructs a portion of communities randomly to start new
   *    evolutions.
   *
   *@param bprop Probability of a community being generated for
   *             a new evolution in any given time window
   */
  void birthEvents ( double bprop );
  
  /**
   *@fn void growAndShrink ( double pgr, double sgr )
   *
   *    Goes through each community. With pgr probability, the 
   * community is chosen to grow. Otherwise it shrinks. Amount of
   * change is up to a sgr percentage of the size of the community.
   *
   *@param pgr Probability a community grows
   *@param sgr Maximum percentage of size that a community can
   *           either grow or shrink
   */
  void growAndShrink ( double pgr, double sgr );

  /**
   *@fn void mergeAndSplit ( double merge_prob, double split_prob, double duplicate_prob, int min_split_size, string filename )
   *
   *
   *@param merge_prob Community is chosen for merge with
   *                   probability ( 1 / (|C|^ ( merge_prob ) ) )
   *@param split_prob If not merged. Community is split with
   *                   probability ( |C| * split_prob )
   *@param duplicate_prob When splitting, this is the probability
   *                      that a vertex moved in the split also
   *                      stays in original community
   *@param min_split_size Minimize size of a community to consider
   *                      for splitting
   */
  void mergeAndSplit ( double merge_prob, double split_prob, double duplicate_prob, int min_split_size, string filename );
};

#endif
