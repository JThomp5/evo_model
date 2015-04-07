#include "VertexGenerator.h"

unsigned int next_int ( unsigned int base ){
  return base + 1;
}

string next_string ( string base ){
  static set < string > the_list;
  the_list.insert( "James" );
  the_list.insert( "Megan" );
  the_list.insert( "Collin" );
  the_list.insert( "Anne" );
  the_list.insert( "Matt" );
  the_list.insert( "Jessie" );
  
  set < string >::iterator it_s;
  if ( ( it_s = the_list.find(base) ) != the_list.end() ){
    if ( ( ++it_s ) != the_list.end() ) {
      return *it_s;
    }
  }

  return *(the_list.begin());
}

string next_string2 ( string base ){
  static map < string, int > lookup;
  vector < string > the_list;
  
  the_list.push_back ( "James" );
  lookup.insert ( pair < string, int > ( "James", 0) );
  the_list.push_back ( "Megan" );
  lookup.insert ( pair < string, int > ( "Megan", 1) );
  the_list.push_back ( "Collin" );
  lookup.insert ( pair < string, int > ( "Collin", 2) );
  the_list.push_back ( "Anne" );
  lookup.insert ( pair < string, int > ( "Anne", 3) );
  the_list.push_back ( "Matt" );
  lookup.insert ( pair < string, int > ( "Matt", 4) );
  the_list.push_back ( "Jessie" );
  lookup.insert ( pair < string, int > ( "Jessie", 5) );

  map < string, int >::iterator it_l;
  if ( ( it_l = lookup.find ( base ) ) == lookup.end() ){
    return the_list[0];
  } else {
    return the_list[(it_l->second + 1 ) % the_list.size()];
  }
}
