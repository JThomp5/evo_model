/**
 *@file Network.cc
 *
 *Member function definitions for the Network class
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

#include "Network.h"

void Network::RandomNetwork ( unique_ptr < Parameters >& P ) { 
  
  //Goes through and initializes each vertex individually         
  unsigned int num_vert = P->get < unsigned int > ( "V", 1000 );
  for ( unsigned int i = 0; i < num_vert; i++ ){
    addRandomVertex();
  }
  
  //Create community structure with randomly sampled vertices
  if ( P->hasFlag ( "cnum" ) ) {
    //Constructs a certain number of communities
    int target = P->get < int > ( "cnum" );
    for ( int i = 0; i < target; i++ ){
      addCommunity ( RandomCommunity ( cpl_->Sample() ) );
    }
  } else {
    //Constructs communities until vertices have a target average
    //    membership
    double target_membership = P->get < double > ( "vmem", 1.2 );
    unsigned int total_size = 0;
    
    while ( total_size < ( target_membership * NumVerts() ) ){
      unsigned int next_size = cpl_->Sample();
      total_size += next_size;

      addCommunity ( RandomCommunity ( next_size ) );
    }
  }

  //Make sure each vertex has at least one community membership
  fillCommunities();
  
  //Construct edge structure
  populateEdges(P);
}

shared_ptr < Community > Network::RandomCommunity ( unsigned int size ) {
  // Empty result community seed
  shared_ptr < Community > res ( new Community() );

  // Continually adds members to community until target size is reached
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
  
  //Goes through each pair of vertices that shares a community
  //  multiple edges are taken care of by the automatic 
  //  uniqueness of stl set containers and custom comparators
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

void Network::addRandomVertex ( ){
  //Generates new energy
  double next_energy = vpl_->Sample();
  total_energy_ += next_energy;
  //Inserts vertex into network
  V_.insert ( shared_ptr < Vertex > ( new Vertex ( next_id_++, next_energy ) ) );
}

void Network::deathEvents ( double dprob ){
  //Goes through each community, deleting it with a given probability
  for ( int i = 0; i < C_.size(); i++ ){
    if ( random_double ( 0, 1 ) < dprob ){
      C_[i]->clearMembers();
    }
  }
}

void Network::birthEvents ( double bprop ){
  //Constructs a relatively small number of new communities
  //    to begin new evolutions
  int new_com = bprop * C_.size();
  for ( int i = 0; i < new_com; i++ ){
    addCommunity ( RandomCommunity ( cpl_->Sample() ) );
  }
}

void Network::growAndShrink ( double pgr, double sgr ){
  for ( int i = 0; i < C_.size(); i++ ){
    int new_size = C_[i]->size();

    //Decides on the new size for the community
    if ( random_double () < pgr ) {
      new_size += (random_double ( 0, sgr ) * new_size );
    } else { 
      new_size -= (random_double ( 0, sgr ) * new_size );
    }
    
    //Changes membership until the sizes match
    while ( C_[i]->size() > new_size ){
      C_[i]->removeRandomMember();
    }

    while ( C_[i]->size() < new_size ){
      C_[i]->addMember( getRandomVertex() );
    }

  }
}

void Network::mergeAndSplit ( double merge_prob, double split_prob, double duplicate_prob, int min_split_size, string filename ){
  ofstream fout ( filename.c_str() ); 
  
  vector < int > merge_coms;
  
  //Goes through each community, picking out communities for 
  //    merging and splitting others.
  for ( int i = 0; i < C_.size(); i++ ){
    double community_fate = random_double();
    
    if ( ( merge_prob > 0 ) && (community_fate < ( 1 / pow ( C_[i]->size(), merge_prob ) ) ) ){
      merge_coms.push_back ( i );
    } else if ( ( C_[i]->size() >= min_split_size) && ( community_fate < ( (merge_prob == 0) ? 0 : ( 1 / pow ( C_[i]->size(), merge_prob ) ) ) + min ( ( C_[i]->size() * split_prob ), 1.0 ) ) ){
      fout << i << " " << i << " " << C_.size() << endl;
      
      uint new_split_size = random_int ( 3, C_[i]->size() - 3 );
      vector < uint > verts_saved;
      
      shared_ptr < Community > split_com ( new Community() );
      for ( uint j = 0; j < new_split_size; j++ ){
	if ( random_double() < duplicate_prob ){
	  split_com->addMember( const_cast < shared_ptr < Vertex >& > ( C_[i]->getRandomMember() ) );
	} else {
	  split_com->addMember( C_[i]->removeRandomMember() );
	}
      }
    }
  }

  //Randomly pairs up communities for merging
  random_shuffle ( merge_coms.begin(), merge_coms.end() );
  
  for ( uint i = 0; i < merge_coms.size() - 1; i+=2 ){
    fout << merge_coms[i+1] << " " << merge_coms[i] << endl;

    const vset old_com = C_[merge_coms[i+1]]->getMembers();
    vset::const_iterator it_v;
    for ( it_v = old_com.begin(); it_v != old_com.end(); it_v++ ){
      C_[merge_coms[i]]->addMember( *it_v );
    }

    C_[merge_coms[i+1]]->clearMembers();
  }
  
  fout.close();
}

void Network::genNextTimeWindow ( unique_ptr < Parameters >& P ){
  //Grows network
  double increment = random_double ( P->get < double > ( "vnewmin", 0.2 ), P->get < double > ( "vnewmax", 0.4 ) );
  unsigned int vadd = V_.size() * increment;
  for ( int i = 0; i < vadd; i++ ){
    addRandomVertex();
  }
  
  //Embeds community events
  deathEvents ( P->get < double > ( "cdie", 0.1 ) );
  growAndShrink ( P->get < double > ( "pgrow" , 0.5 ), P->get < double > ( "maxgrow", 0.25 ) );
  mergeAndSplit ( P->get < double > ( "pmerge", 1), P->get < double > ( "psplit", 0.01), P->get < double > ( "dup", 0.2 ),  P->get < int > ( "minsplit", 7 ), P->get < string > ( "fout", "Transition" ) + to_str < int > ( current_window_ ) + "-" + to_str < int > (current_window_+1) );
  birthEvents ( P->get < double > ( "cnew", 0.1 ) );

  //Makes sure each vertex is still in a community
  fillCommunities();
  
  //Constructs network
  populateEdges(P);

  //Incrementstracker
  ++current_window_;
}

void Network::fillCommunities (){
  vset::iterator it_v = V_.begin();
  
  //Adds random communities to the structure until
  //   each vertex is associated with at least one community
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

void Network::printNetwork ( string filename ){
  ofstream fout ( filename.c_str() );
  
  eset::iterator it_e;
  //If the edge representation is not empty ( 0-weight edge )
  //    print the string representation to the file provided
  for ( it_e = E_.begin(); it_e != E_.end(); it_e++ ){
    if ( ( (*it_e)->toString() ).size() > 0 )
      fout << (*it_e)->toString();
  }
  
  fout.close();
}
