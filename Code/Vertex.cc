/**
 *@file Vertex.cc
 *
 * Definitions for member functions of the Vertex class
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

#include "Vertex.h"

Vertex& Vertex::operator=(const Vertex& other){
  //Copies members from other object
  id_ = other.id_;
  energy_ = other.energy_;

  //Return reference to this object for chaining
  return *this;
}
