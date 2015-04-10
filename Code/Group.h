#ifndef RPI_GROUP
#define RPI_GROUP

#include "Vertex.h"

using namespace std;

class Group{
 public:
  Group();
  Group( const Group* other );
  Group( const Group& other );
  virtual ~Group();

  bool operator< ( const Group& other );
  Group& operator= ( const Group& other );
  
  bool addMember ( shared_ptr < Vertex > V );
  bool hasMember ( shared_ptr < Vertex > V );
  
  virtual string toString();
  unsigned int size() { return members_.size(); }
  const vset& getMembers () { return members_; }
  
 protected:
  vset members_;
};

#endif
