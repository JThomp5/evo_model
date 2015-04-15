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
  
  for ( unsigned int i = 1; i < t; i++ ){
    cout << "Constructing window " << i << endl;
    N->genNextTimeWindow( P );
    N->printNetwork ( "Network" + to_str < unsigned int > ( i ) + ".dat" );
  }

  //Iteratively constructs following time windows, 
  //   printing out the information as it goes
  
}
