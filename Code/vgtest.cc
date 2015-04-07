#include <iostream>
#include "VertexGenerator.h"

using namespace std;

int main ( int argc, char** argv ){
  VertexGenerator <string> vg("Collin", next_string2);
  
  for ( int i = 0; i < 5; i++ ){
    cout << vg.next().getID() << endl;
  }
}
