#ifndef CommunicationSocket_h
#define CommunicationSocket_h

#include "SocketImplBase.h"

class CommunicationSocket : SocketImplBase
{
   friend class SocketImplBase;

private:
   CommunicationSocket(int& Fd, const sockaddr_in& SockAddr, bool nonblocking)
      : SocketImplBase(Fd, SockAddr, nonblocking)
   {
   }

public:
   using SocketImplBase::Read;
   using SocketImplBase::Write;
   using SocketImplBase::SocketFD;
};

#endif
