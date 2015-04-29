/**
 *@file Group.h
 *
 * Definitions of Group class.
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

#ifndef RPI_GROUP
#define RPI_GROUP

#include "Vertex.h"
#include <iostream>

using namespace std;

class Group{
 public:
  /**
   *@fn Group()
   *   Empty
   */
  Group();

  /**
   *@fn Group( const Group* other )
   *@fn Group( const Group& other )
   *
   * Copy constructors. Will deep copy members of other Group
   */
  Group( const Group* other );
  Group( const Group& other );
  
  /**
   *@fn ~Group()
   *   Empty
   */
  virtual ~Group();

  /**
   *@fn bool operator< ( const Group& other )
   *
   * Custom comparator. Uses the comparators of members to compare.
   *
   *@return True if this Group comes before the other, in the order
   */
  bool operator< ( const Group& other );

  /**
   *@fn Group& operator= ( const Group& other )
   *
   *  Assignment operator. Deep copy of members.
   *
   *@return Reference to this object
   */
  Group& operator= ( const Group& other );
  
  /**
   *@fn bool addMember ( shared_ptr < Vertex > V )
   *
   * Add a vertex to the community.
   *
   *@param V Vertex to add to community
   */
  bool addMember ( shared_ptr < Vertex > V );
  
  /**
   *@fn bool hasMember ( shared_ptr < Vertex > V )
   *
   * Check if a community has a vertex
   *
   *@param V Vertex to check for
   *@return True if V is already in the community
   */
  bool hasMember ( shared_ptr < Vertex > V );

  /**
   *@fn shared_ptr < Vertex > removeRandomMember()
   *
   * Removes a random vertex from the community
   *
   *@return Vertex removed
   */
  shared_ptr < Vertex > removeRandomMember();
  
  /**
   *@fn const shared_ptr < Vertex >& getRandomMember()
   *
   *    Retreives a random vertex in the community.
   *
   *@return Chosen vertex
   */
  const shared_ptr < Vertex >& getRandomMember();
  
  /**
   *@fn void clearMembers ( )
   *
   * Removes all members from the community
   */
  void clearMembers ( );
  
  /**
   *@fn virtual string toString()
   *
   *  Constructs a string representation of the group
   *
   *@return string representation of the Group 
   */
  virtual string toString();
  
  /**
   *@fn unsigned int size()
   *
   *@return Number of members in the group
   */
  unsigned int size() { return members_.size(); }
  
  /**
   *@fn const vset& getMembers ()
   *
   *@return Set of the members of the group
   */
  const vset& getMembers () { return members_; }
  
 protected:
  vset members_; //Members of the group
};

#endif
