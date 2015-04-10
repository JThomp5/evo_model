#include "Group.h"

Group::Group ( ) {};

Group::Group ( const Group* other ){
  vset::iterator it_s;
  for ( it_s = other->members_.begin(); it_s != other->members_.end(); it_s++ ){
    members_.insert ( *it_s );
  }
}

Group::Group ( const Group& other ) {
  vset::iterator it_s;
  for ( it_s = other.members_.begin(); it_s != other.members_.end(); it_s++ ){
    members_.insert ( *it_s );
  }
}

Group::~Group ( ) {
}

bool Group::operator< ( const Group& other ){
  vset::iterator it_s, it_t;
  for ( it_s = members_.begin(), it_t = other.members_.begin(); (it_s != members_.end()) && (it_t != other.members_.end()); it_s++, it_t++ ){
    if ( **it_s < **it_t ){
      return true;
    } else if ( **it_t < **it_s ) {
      return false;
    }
  }

  if ( it_s != members_.end() ){
    return false;
  } else if ( it_t != other.members_.end() ){
    return true;
  } else {
    return false;
  }
}

Group& Group::operator= ( const Group& other ){
  vset::iterator it_s;
  for ( it_s = other.members_.begin(); it_s != other.members_.end(); it_s++ ){
    members_.insert ( *it_s );
  }

  return *this;
}

bool Group::addMember ( shared_ptr < Vertex > V ){
  pair < vset::iterator, bool > res = members_.insert ( V );
  return res.second;
}

bool Group::hasMember ( shared_ptr < Vertex >  V ){
  return ( members_.find ( V ) != members_.end() );
}

string Group::toString() {
  vset::iterator it_v;
  string res = "( ";
  for ( it_v = members_.begin(); it_v != members_.end(); it_v++ ){
    res += (*it_v)->toString() + " ";
  }
  res += ")";

  return res;
}