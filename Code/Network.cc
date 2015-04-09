#include "Network.h"

bool RandomNetwork ( unique_ptr < Network >& N, unique_ptr < Parameters >& P ) {
  unique_ptr < PowerLaw > cpl ( new PowerLaw ( -(P->get < double > ( "cexp" )), P->get < double > ( "cmin" ), P->get < double > ( "cmax" ) ) );

  if ( P->hasFlag ( "cnum" ) ) {
    int target = P->get < int > ( "cnum" );
    for ( int i = 0; i < target; i++ ){
      N->addCommunity ( RandomCommunity ( cpl->Sample(), N ) );
    } 
  } else {
    double target_membership = P->get < double > ( "vmem", 1.2 );
    unsigned int total_size = 0;
    
    while ( total_size < ( target_membership * N->NumVerts() ) ){
      unsigned int next_size = cpl->Sample();
      total_size += next_size;
      N->addCommunity ( RandomCommunity ( next_size, N ) );
    }
  }
}

shared_ptr < Community > RandomCommunity ( unsigned int size, unique_ptr < Network >& N ) {
  shared_ptr < Community > res ( new Community() );
  for ( int i = 0; i < size; i++ ){
    res->addMember ( N->getRandomVertex () );
  }
  
  return res;
}
