#ifndef SocketImplBase_h
#define SocketImplBase_h

#include <netinet/in.h>

#include <boost/shared_ptr.hpp>
#include <string>
#include <vector>

class CommunicationSocket;

class SocketImplBase
{
protected:
   explicit SocketImplBase(const std::string& endpoint /* ip:port */, bool nonblocking);
   explicit SocketImplBase(int& fd, const sockaddr_in& SockAddr, bool nonblocking);
   virtual ~SocketImplBase();

   void Listen(int backlog);
   void Connect(const std::string& rendpoint);

   void NonBlocking(bool Set);
   bool NonBlocking() const;

   void Read(std::vector<char>& IBuf);
   void Write(std::vector<char>& OBuf);

   boost::shared_ptr<CommunicationSocket> Talk();
   boost::shared_ptr<CommunicationSocket> Accept();

   int SocketFD() const;

private:
   SocketImplBase(const SocketImplBase&) /* = delete */;
   void operator=(const SocketImplBase&) /* = delete */;

private:
   static void ThrowError(const std::string& endpoint, const std::string& context, int lerrno);
   static void ThrowError(const std::string& endpoint, const std::string& context, const std::string& error);

   void ThrowError(const std::string& context, const std::string& error) const;
   void ThrowError(const std::string& context, int lerrno) const;

   static sockaddr_in EndpointToSockAddr(const std::string& endpoint);

   void Bind();

private:
   int m_Fd;
   sockaddr_in m_LocalSockAddr;
};

#endif
