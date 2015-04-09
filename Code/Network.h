#ifndef RPI_NETWORK
#define RPI_NETWORK

#include "Vertex.h"
#include "Community.h"
#include "../../Libraries/Random/PowerLaw.h"
#include "../../Libraries/Random/Wrappers.h"
#include "../../Libraries/Params/Parameters.h"
#include <set>
#include <tr1/memory>
#include <iostream>

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
  Network ( unique_ptr < Parameters >& P ): total_energy_(0), next_id_(0), vpl_( new PowerLaw ( -P->get < double > ( "vexp", -2.0 ), P->get < double > ( "vmin", 1 ), P->get < double > ( "vmax", 5 ) ) ){
    if ( vpl_->getExp() > 0) { vpl_ = NULL;}

    //Goes through and initializes each vertex individually
    unsigned int num_vert = P->get < unsigned int > ( "V", 10 );
    for ( unsigned int i = 0; i < num_vert; i++ ){
      double next_energy = vpl_->Sample();
      total_energy_ += next_energy;
      V_.insert ( shared_ptr < Vertex > ( new Vertex ( next_id_++, next_energy ) ) );
    }
  }

  ~Network(){};

  void addCommunity( shared_ptr < Community > C ) {
    C_.push_back ( C );
  }

  void printCommunities(){
    for ( int i = 0; i < C_.size(); i++ ){
      cout << C_[i]->toString() << endl;
    }
  }

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
  unsigned int next_id_;          //Track largest id
  double total_energy_;
  unique_ptr < PowerLaw > vpl_;   //Power law for energy values
};

/**
 *@fn void RandomNetwork ( unique_ptr < Network >& N )
 *
 *@PRECONDITIONS:
 *
 *Constructs a new random community structure and edge structure
 *    for the given network, which must have a vertex structure. If
 *    a community or edge structure exists, this function will not
 *    work.
 *
 *@POSTCONDITIONS:
 *
 *@param N Network with a vertex structure.
 *@return False if an edge or community structure was 
 *       already in place.
 */
bool RandomNetwork ( unique_ptr < Network >& N, unique_ptr < Parameters >& P );
shared_ptr < Community > RandomCommunity ( unsigned int size, unique_ptr < Network >& N );

#endif
