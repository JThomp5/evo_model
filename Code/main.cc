#include <iostream>

#include "../../Libraries/Params/Parameters.h"
#include "VertexGenerator.h"
#include "Network.h"

using namespace std;

int main ( int argc, char** argv ){
  unique_ptr < Parameters > P ( new Parameters () );
  P->Read(argc, argv);
 

  if ( P->get<char> ( "type", 'u' ) == 's' ){
    Network < string > N ( P );
  } else {
    Network < unsigned int > N ( P );
  }
 
  
}
