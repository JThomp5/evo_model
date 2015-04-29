/**
 *@file Group.cc
 * 
 *    Definitions of the member functions for the Group class.
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

#include "Group.h"

Group::Group ( ) {};

Group::Group ( const Group* other ){
  //Copies over members of other group
  vset::iterator it_s;
  for ( it_s = other->members_.begin(); it_s != other->members_.end(); it_s++ ){
    members_.insert ( *it_s );
  }
}

Group::Group ( const Group& other ) {
  //Copies over member of other group
  vset::iterator it_s;
  for ( it_s = other.members_.begin(); it_s != other.members_.end(); it_s++ ){
    members_.insert ( *it_s );
  }
}

Group::~Group ( ) {
}

bool Group::operator< ( const Group& other ){
  //Goes through members of each group. The first time two members
  //  are different, 
  vset::iterator it_s, it_t;
  for ( it_s = members_.begin(), it_t = other.members_.begin(); (it_s != members_.end()) && (it_t != other.members_.end()); it_s++, it_t++ ){
    if ( **it_s < **it_t ){
      return true;
    } else if ( **it_t < **it_s ) {
      return false;
    }
  }

  if ( it_s != members_.end() ){
    return false;
  } else if ( it_t != other.members_.end() ){
    return true;
  } else {
    return false;
  }
}

Group& Group::operator= ( const Group& other ){
  //Copies over the members of the other group
  vset::iterator it_s;
  for ( it_s = other.members_.begin(); it_s != other.members_.end(); it_s++ ){
    members_.insert ( *it_s );
  }

  return *this;
}

bool Group::addMember ( shared_ptr < Vertex > V ){
  //Simply inserts the vertex into the member set
  pair < vset::iterator, bool > res = members_.insert ( V );
  return res.second;
}

bool Group::hasMember ( shared_ptr < Vertex >  V ){
  //Checks if the group includes vertex V
  return ( members_.find ( V ) != members_.end() );
}

const shared_ptr < Vertex >& Group::getRandomMember (){
  //Retreive a random member from the group
  int choice = rand() % members_.size();
  vset::iterator it_v = members_.begin();
  
  for ( int i = 0; i < choice; i++ ){
    ++it_v;
  }
  
  return *it_v;
}

shared_ptr < Vertex > Group::removeRandomMember(){
  //Chooses a member randomly from the group first
  int choice = rand() % members_.size();
  vset::iterator it_v = members_.begin();
  
  for ( int i = 0; i < choice; i++ ){
    ++it_v;
  }

  shared_ptr < Vertex > res = const_cast < shared_ptr < Vertex > & >(*it_v);

  //Removes member from group
  members_.erase ( it_v );
  
  return res;
}

void Group::clearMembers ( ){
  members_.clear();
}

string Group::toString() {
  //Default representation : space delimited members surrounded by
  //   parentheses
  vset::iterator it_v;
  string res = "( ";
  for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
    res += (*it_v)->toString() + " ";
  }
  res += ")";

  return res;
}
