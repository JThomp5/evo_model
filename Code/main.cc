/**
 *@file main.cc
 *
 * Main entry point for temporal social network model. First 
 *    constructs a random static network and then iteratively 
 *    constructs static networks for future time windows. See
 *
 *@author James Thompson
 *
 * Copyright James Thompson 2015
 * This program is distributed under the terms of the GNU General Public License
 *
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

#include <iostream>

#include "../../Libraries/Params/Parameters.h"
#include "Network.h"

using namespace std;

int main ( int argc, char** argv ){
  //Reads in the command line arguments
  unique_ptr < Parameters > P ( new Parameters () );
  P->Read(argc, argv);
  
  //Creates the first time window's static network
  unique_ptr < Network> N ( new Network ( P ) );
  N->RandomNetwork ( P );
  N->printNetwork ( "Network0.dat" );
  
  unsigned int t = P->get < unsigned int > ( "t", 10 );
  
  //Iteratively constructs following time windows, 
  //   printing out the information as it goes
  for ( unsigned int i = 1; i < t; i++ ){
    cout << "Constructing window " << i << endl;
    N->genNextTimeWindow( P );
    N->printNetwork ( "Network" + to_str < unsigned int > ( i ) + ".dat" );
  }  
}
