/**
 *@file Edge.cc
 *
 *  Definitions for Edge member functions. 
 *
 *@author James Thompson
 *
 *
 * Copyright 2015 James Thompson
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

#include "Edge.h"

bool Edge::generateWeight ( unique_ptr < Parameters > & P ) {
  //Sets up power law for edge frequency - will change
  //  based off of how many other edges the members are 
  //  actively involved in. This means the same edge might have
  //  different lag/energy distributions in different time windows
  PowerLaw pl ( -1.75, getTotalEnergy(P->get < double > ( "grav",0.2 ), P-> get < double > ( "minlag", 0.2 ), P->get < double > ( "vmax", 1 ) ), 3.0 ); 
  edge_weight_ = 0;
  
  //wait_time_ represents the time at which the next interaction 
  //   between members of the edge will happen ( sorry for the
  //   slight misnomer ). If wait_time is 0.27, it means that 
  //   the next interaction will happen 0.27 time units into a 
  //   time window [0, 1)
  while ( wait_time_ < 1.0 ){
    wait_time_ += pl.Sample();
    ++edge_weight_;
  }

  //Track that the time window has passed
  wait_time_ -= 1.0;

  //Increment the number of active edges the members of this edge
  //    are involved in this edge if at least interaction has 
  //    happened in the current time window.
  if ( edge_weight_ > 0 ){
    vset::iterator it_v;
    for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
      (*it_v)->incrementEdgeCount();
    }
  }

  //Return true if an interaction happened
  return edge_weight_ > 0;
}

double Edge::getTotalEnergy ( double gravity, double minlag, double max_energy ){
  double res = -1;
  double max_res = 0;
  bool considered = false;

  // Lag is the inverse of energy ( high energy vertices have 
  //      frequent or low lag interactions ). Finds the max lag.
  vset::iterator it_v;
  for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
    double v_lag = ( max_energy - (*it_v)->getEnergy() ) + minlag;
    if ( v_lag > res ) {
      res = v_lag;
    }
  }

  max_res = res;

  // Adds influence of lag values that are not the maximum. 
  //    The idea is that a high lag - low lag vertex edge should
  //    connect more than a two-low-lag-vertex edge. 
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

string Edge::toString ( ) {
  string res = "";

  //If there is no weight on the edge, it's not considered an edge.
  if ( edge_weight_ == 0 ) return res;
  
  //Constructs a 'to|from|weight' representation of the edge.
  vset::iterator it_a, it_b;
  for ( it_a = members_.begin(); it_a != members_.end(); it_a++ ){
    it_b = it_a;
    for ( ++it_b; it_b != members_.end(); it_b++ ){
      res += (*it_a)->toString() + "|" + (*it_b)->toString() + "|" + to_str < double > ( edge_weight_ ) + "\n";
    }
  }
  
  return res;
}
