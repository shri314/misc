#ifndef ConnectingSocket_h
#define ConnectingSocket_h

#include "utils/SocketImplBase.h"

class ConnectingSocket : SocketImplBase
{
public:
   ConnectingSocket(const std::string& rendpoint /* 127.0.0.1:9999 */, const std::string& lendpoint, bool nonblocking = false)
      : SocketImplBase(lendpoint, nonblocking)
   {
      Connect(rendpoint);
   }

   ConnectingSocket(const std::string& rendpoint /* 127.0.0.1:9999 */, bool nonblocking = false)
      : SocketImplBase("0.0.0.0:0", nonblocking)
   {
      Connect(rendpoint);
   }

   using SocketImplBase::Talk;
   using SocketImplBase::SocketFD;
};

#endif
