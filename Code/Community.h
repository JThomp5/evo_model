#ifndef RPI_COM
#define RPI_COM

#include "Vertex.h"

class Community{
 public:
  Community();
  Community( const Community* other );
  Community( const Community& other );
  ~Community();

  bool operator< ( const Community& other );
  Community& operator= ( const Community& other);

  bool addMember ( shared_ptr < Vertex > V );
  bool hasMember ( shared_ptr < Vertex > V );

  string toString();

 private:
  vset members_;
};

#endif
