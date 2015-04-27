#ifndef RPI_EDGE
#define RPI_EDGE

#include "Group.h"
#include "../../Libraries/Params/Parameters.h"
#include "../../Libraries/Random/PowerLaw.h"

using namespace std;

class Edge:public Group {
 public:  
 Edge():Group(), wait_time_(0), edge_weight_(0){}
  ~Edge(){}
  
  bool generateWeight(unique_ptr < Parameters >& P ) {
    PowerLaw pl ( -1.75, getTotalEnergy(P->get < double > ( "grav",0.2 ), P-> get < double > ( "minlag", 0.2 ), P->get < double > ( "vmax", 1 ) ), 3.0 );
    edge_weight_ = 0;
    
    while ( wait_time_ < 1.0 ){
      wait_time_ += pl.Sample();
      ++edge_weight_;
    }

    wait_time_ -= 1.0;

    if ( edge_weight_ > 0 ){
      //Increase edge count for all vertices in this edge
      vset::iterator it_v;
      for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
	(*it_v)->incrementEdgeCount();
      }
    }

    return edge_weight_ > 0;
  }

  double getTotalEnergy( double gravity, double minlag, double max_energy){
    double res = -1;
    double max_res = 0;
    bool considered = false;
    
    vset::iterator it_v;
    for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
      double v_lag = ( max_energy - (*it_v)->getEnergy() ) + minlag;
      if ( v_lag > res ) {
	res = v_lag;
      }
    }
    
    max_res = res;

    for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
      double v_lag = ( max_energy - (*it_v)->getEnergy() ) + minlag;
      if ( ( v_lag == max_res ) && (!considered) ){
	considered = true;
	continue;
      } else {
	res -= ( gravity * ( res - v_lag ) );
      }
    }
    
    return res;
  }

  double getWeight ( ) { return edge_weight_; }
  
  string toString() {
    string res = "";
    
    if ( edge_weight_ == 0 ) return res;

    vset::iterator it_a, it_b;
    for ( it_a = members_.begin(); it_a != members_.end(); it_a++ ){
      it_b = it_a;
      for ( ++it_b; it_b != members_.end(); it_b++ ){
	res += (*it_a)->toString() + "|" + (*it_b)->toString() + "|" + to_str < double > ( edge_weight_ ) + "\n";
      }
      
    }
    
    return res;
  }

 private:
  double wait_time_;
  double edge_weight_;
};

struct cmp_pedge{
  bool operator() ( const shared_ptr < Edge >& A, const shared_ptr < Edge >& B ){
    return ( *A < *B );
  }
};

typedef set < shared_ptr < Edge >, cmp_pedge > eset;

#endif
