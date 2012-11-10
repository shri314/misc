#ifndef ListeningSocket_h
#define ListeningSocket_h

#include "SocketImplBase.h"

class ListeningSocket : SocketImplBase
{
public:
   explicit ListeningSocket(const std::string& endpoint /* 0.0.0.0:9999 */, bool nonblocking = false, int backlog = 10)
      : SocketImplBase(endpoint, nonblocking)
   {
      Listen(backlog);
   }

   using SocketImplBase::Accept;
   using SocketImplBase::SocketFD;
};

#endif
