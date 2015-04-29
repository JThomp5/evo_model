/**
 *@file Community.h
 *
 *   Quick representation of a community - direct extension of
 * group. Could have been a typedef up to now.
 *
 *@author James Thompson
 *
 *
 * Copyright 2015 James Thompson
 * This program is distributed under the terms of the GNU General Public License

 *This file is part of RPI-evo-model.                              
                                                                    
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

#ifndef RPI_COM
#define RPI_COM

#include "Group.h"

/**
 *@class Community:public Group
 *
 * A community is simply a group of vertices that (hopefully)
 *    have a larger meaning in the context of a network - either
 *    a shared function or characteristic or something similar.
 */
class Community:public Group{
};

#endif
