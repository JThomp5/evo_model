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
  RandomNetwork ( N, P );
  N->printCommunities();
  
  //Iteratively constructs following time windows, 
  //   printing out the information as it goes
  
}
