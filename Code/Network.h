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

  ~Network(){};

  void RandomNetwork ( unique_ptr < Parameters >& P );
  shared_ptr < Community > RandomCommunity ( unsigned int size );

  void addCommunity( shared_ptr < Community > C ) {
    C_.push_back ( C );
    
    const vset c_mem = C->getMembers();
    vset::const_iterator it_c;
    for ( it_c = c_mem.begin(); it_c != c_mem.end(); it_c++ ){
      membership_.insert(pair < shared_ptr < Vertex >, int > ( *it_c, C_.size() - 1 ) );
    }
  }

  void printCommunities(){
    for ( int i = 0; i < C_.size(); i++ ){
      cout << C_[i]->toString() << endl;
    }
  }

  void printEdges ( ) {
    eset::iterator it_e;
    for ( it_e = E_.begin(); it_e != E_.end(); it_e++ ){
      cout << (*it_e)->toString() << " " << (*it_e)->getWeight() << endl;
    }
  }

  void fillCommunities( );
  void populateEdges ( unique_ptr < Parameters >& P );

  /**
   *THIS FUNCTION IS FOR DEBUGGING AND CAN BE SAFELY REMOVED 
   *    (PROBABLY)
   */
  void printVertices() {
    set < shared_ptr < Vertex > >::iterator it_v;
    
    for ( it_v = V_.begin(); it_v != V_.end(); it_v++ ){
      cout << (*it_v)->toString() << " : " << (*it_v)->getEnergy() <<  endl;
    }
  };

  /**
   *
   */
  unsigned int NumVerts ( ){
    return V_.size();
  }

  void printVerts(){
    vset::iterator it_v = V_.begin(); 
    while ( it_v != V_.end() ){
      cout << (*it_v)->toString() << " ";

      ++it_v;
    }
    cout << endl;
  }

  void printNetwork ( string filename );

  void addRandomVertex ( );
  void genNextTimeWindow( unique_ptr < Parameters >& P );

  void deathEvents ( double dprob );
  void birthEvents ( double bprop );
  void growAndShrink ( double pgr, double sgr );
  void mergeAndSplit ( double merge_prob, double split_prob, double duplicate_prob, int min_split_size, string filename );

  shared_ptr < Vertex > getRandomVertex ( ){
    double energy_seen = 0;
    double random_target = random_double() * total_energy_;
    
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
  vector < shared_ptr < Community > > C_;
  eset E_;
  map < shared_ptr < Vertex >, int, cmp_vptr > membership_;
  unsigned int next_id_;          //Track largest id
  double total_energy_;
  unique_ptr < PowerLaw > vpl_;   //Power law for energy values
  unique_ptr < PowerLaw > cpl_;   //Communty sizes
  
  int current_window_;
};

#endif
