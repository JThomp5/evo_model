#include "Network.h"

void Network::RandomNetwork ( unique_ptr < Parameters >& P ) { 
  
  //Goes through and initializes each vertex individually         
  unsigned int num_vert = P->get < unsigned int > ( "V", 10 );
  for ( unsigned int i = 0; i < num_vert; i++ ){
    double next_energy = vpl_->Sample();
    total_energy_ += next_energy;
    V_.insert ( shared_ptr < Vertex > ( new Vertex ( next_id_++, next_energy ) ) );
  }
  
  //Create community structure                                    
  if ( P->hasFlag ( "cnum" ) ) {
    int target = P->get < int > ( "cnum" );
    for ( int i = 0; i < target; i++ ){
      addCommunity ( RandomCommunity ( cpl_->Sample() ) );
    }
  } else {
    double target_membership = P->get < double > ( "vmem", 1.2 );
    unsigned int total_size = 0;
    
    while ( total_size < ( target_membership * NumVerts() ) ){
      unsigned int next_size = cpl_->Sample();
      total_size += next_size;

      addCommunity ( RandomCommunity ( next_size ) );
    }
  }

  fillCommunities(P);
  
  populateEdges(P);
}

shared_ptr < Community > Network::RandomCommunity ( unsigned int size ) {
  shared_ptr < Community > res ( new Community() );


  for ( int i = 0; i < size; i++ ){
    if ( !res->addMember ( getRandomVertex () ) ){
      --i; 
    }
  }
  return res;
}

void Network::populateEdges ( unique_ptr < Parameters >& P ){
  //Generates internal edges, copying old edge if exists
  eset new_edge_set;
  eset::iterator it_e; 
  for ( int i = 0; i < C_.size(); i++ ){
    const vset verts = C_[i]->getMembers();
    
    vset::const_iterator it_a, it_b;
    for ( it_a = verts.begin(); it_a != verts.end(); it_a++ ){
      it_b = it_a;
      for (++it_b; it_b != verts.end(); it_b++ ){
	shared_ptr < Edge > new_edge ( new Edge () );
	new_edge->addMember ( *it_a );
	new_edge->addMember ( *it_b );
	new_edge->generateWeight( P ); //Initializes the edge with
	                               //   a non-zero wait time
	if ( ( it_e = E_.find ( new_edge ) ) != E_.end() ){
	  new_edge_set.insert ( *it_e );
	} else {
	  new_edge_set.insert ( new_edge );
	}
      }
    }
  }
 
  //Generates external edges, copying old if exists
  double mixing_parameter = P->get < double > ( "mp", 0.85 );
  int edges_to_generate = ( (1.0 - mixing_parameter) / mixing_parameter ) * new_edge_set.size();

  for ( int i = 0; i < edges_to_generate; i++ ){
    shared_ptr < Edge > new_edge ( new Edge() );
    new_edge->addMember ( getRandomVertex() );
    new_edge->addMember ( getRandomVertex() );
    new_edge->generateWeight ( P );
    
    //Makes sure the edge is external
    if ( ( it_e = new_edge_set.find ( new_edge ) ) != E_.end () ){
      continue;
    }
 
    //Adds old edge if it was already active ( low probaility, but still...)
    if ( ( it_e = E_.find ( new_edge ) ) != E_.end() ){
      new_edge_set.insert ( *it_e );
    } else {
      new_edge_set.insert ( new_edge );
    }
  }
  
  //Copies new edge set to network
  E_ = new_edge_set;
  
  //Resets edge counts for vertices
  vset::iterator it_v;
  for ( it_v = V_.begin(); it_v != V_.end(); it_v++ ){
    (*it_v)->resetEdgeCount();
  }
  
  //Generates new weights for all edges
  for ( it_e = E_.begin(); it_e != E_.end(); it_e++ ){
    (*it_e)->generateWeight ( P );
  }
}

void Network::genNextTimeWindow ( unique_ptr < Parameters >& P ){
  //addRandomVertices ( random_double ( 0, P->get < double > ( "vnew", 0.1) ));
  
  //deathEvents ( P->get < double > ( "cdie", 0.1 ) );
  //growAndShrink ( P->get < double > ( "pgrow" , 0.5 ), P->get < double > ( "maxgrow", 0.25 ) );
  //mergeAndSplit ( P->get < double > ( "pmerge", ), P->get < double > ( "psplit", ) );
  //birthEvents ( P->get < double > ( "cnew", 0.1 ) );
}

void Network::fillCommunities ( unique_ptr < Parameters >& P ){
  vset::iterator it_v = V_.begin();
  
  while ( membership_.size() != V_.size() ){
    shared_ptr < Community > next_com ( new Community() );
    unsigned int next_size = cpl_->Sample();

    while ( ( it_v != V_.end() ) && (next_com->size() < next_size ) ){
      if ( membership_.find (*it_v) == membership_.end() ){
	next_com->addMember ( *it_v );
      }
      ++it_v;
    }
    
    if ( ( it_v == V_.end() ) && ( next_com->size() < next_size ) ){
      while ( next_com->size() < next_size ){
	next_com->addMember ( getRandomVertex () );
      }
    }
    
    addCommunity ( next_com );
  }
}
