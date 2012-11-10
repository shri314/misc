#include "CommunicationSocket.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include <boost/lexical_cast.hpp>

#include <cerrno>
#include <stdexcept>

SocketImplBase::SocketImplBase(const std::string& lendpoint /* ip:port */, bool nonblocking)
   : m_Fd(-1)
   , m_LocalSockAddr( EndpointToSockAddr(lendpoint) )
{
   m_Fd = socket(PF_INET, SOCK_STREAM, 0);

   Bind();

   NonBlocking(nonblocking);

   if(m_Fd == -1)
      ThrowError("Count not create socket", errno);
}

SocketImplBase::SocketImplBase(int& fd, const sockaddr_in& SockAddr, bool nonblocking)
   : m_Fd(-1)
   , m_LocalSockAddr( SockAddr )
{
   std::swap(m_Fd, fd);

   NonBlocking(nonblocking);
}

SocketImplBase::~SocketImplBase()
{
   close(m_Fd);
}

void SocketImplBase::ThrowError(const std::string& endpoint, const std::string& context, const std::string& error)
{
   throw std::runtime_error( std::string("[") + endpoint + "][" + context + "][" + error + "]");
}

void SocketImplBase::ThrowError(const std::string& endpoint, const std::string& context, int lerrno)
{
   ThrowError(endpoint, context, lerrno != 0 ? strerror(lerrno) : "");
}

void SocketImplBase::ThrowError(const std::string& context, const std::string& error) const
{
   char buf[INET_ADDRSTRLEN] = {};
   std::string ip = inet_ntop(AF_INET, &m_LocalSockAddr, buf, sizeof m_LocalSockAddr);
   std::string port = boost::lexical_cast<std::string>(ntohs(m_LocalSockAddr.sin_port));

   ThrowError( ip + ":" + port, context, error );
}

void SocketImplBase::ThrowError(const std::string& context, int lerrno) const
{
   ThrowError(context, lerrno != 0 ? strerror(lerrno) : "");
}

sockaddr_in SocketImplBase::EndpointToSockAddr(const std::string& endpoint)
{
   std::string::size_type p = endpoint.find(':');
   std::string ip = (p == 0 ? std::string("0.0.0.0") : endpoint.substr(0, p));
   std::string port = (p != std::string::npos ? endpoint.substr(p + 1) : std::string("0"));

   sockaddr_in addr_in = {};

   addr_in.sin_family = AF_INET;

   errno = 0;
   int ret = inet_pton(AF_INET, ip.c_str(), &addr_in.sin_addr.s_addr);
   if(ret == -1)
      ThrowError(endpoint, "error in endpoint", errno);

   try
   {
      addr_in.sin_port = htons( boost::lexical_cast<short>( port ) );
   }
   catch(const std::exception& e)
   {
      ThrowError(endpoint, "error in endpoint", e.what());
   }

   return addr_in;
}

void SocketImplBase::Bind()
{
   errno = 0;
   int ret = bind(m_Fd, (const sockaddr*)&m_LocalSockAddr, sizeof m_LocalSockAddr);
   if(ret == -1)
      ThrowError("Count not bind", errno);
}

void SocketImplBase::Listen(int backlog)
{
   errno = 0;
   int ret = listen(m_Fd, backlog);
   if(ret == -1)
      ThrowError("Count not listen", errno);
}

void SocketImplBase::Connect(const std::string& rendpoint)
{
   sockaddr_in SockAddr = SocketImplBase::EndpointToSockAddr(rendpoint);

   while(true)
   {
      errno = 0;
      int ret = connect(m_Fd, (const sockaddr*)&SockAddr, sizeof SockAddr);
      int lerrno = errno;

      if(ret == -1)
      {
         if(lerrno == EINTR)
            continue;

         if(lerrno == EINPROGRESS || lerrno == EISCONN)
            return;
      }

      break;
   }
   
   ThrowError("Count not connect", errno);
}

void SocketImplBase::NonBlocking(bool Set)
{
   errno = 0;
   int fl = fcntl(m_Fd, F_GETFL, 0);
   if(fl == -1)
      ThrowError("Count not get fcntl flags", errno);

   int ret = fcntl(m_Fd, F_SETFL, Set ? (fl | O_NONBLOCK) : (fl & ~O_NONBLOCK));
   if(ret == -1)
      ThrowError(std::string("Count not ") + (Set ? "set" : "clear") + " O_NONBLOCK flags", errno);
}

bool SocketImplBase::NonBlocking() const
{
   errno = 0;
   int fl = fcntl(m_Fd, F_GETFL, 0);
   if(fl == -1)
      ThrowError("Count not get nonblocking", errno);

   return fl & O_NONBLOCK;
}

void SocketImplBase::Read(std::vector<char>& IBuff)
{
   while(true)
   {
      errno = 0;
      int ret = recv(m_Fd, &IBuff[0], IBuff.size(), 0);
      if(ret == -1)
      {
         if(errno == EINTR)
            continue;

         if(errno == EAGAIN)
            return;

         break;
      }

      IBuff.resize(ret);
   }

   ThrowError("could not recv", errno);
}


void SocketImplBase::Write(std::vector<char>& OBuff)
{
   while(true)
   {
      errno = 0;
      int ret = send(m_Fd, &OBuff[0], OBuff.size(), 0);
      if(ret == -1)
      {
         if(errno == EINTR)
            continue;

         if(errno == EAGAIN)
            return;

         break;
      }

      OBuff.erase(OBuff.begin(), OBuff.begin() + ret);
   }

   ThrowError("could not send", errno);
}


boost::shared_ptr<CommunicationSocket> SocketImplBase::Talk()
{
   return boost::shared_ptr<CommunicationSocket>(
         new CommunicationSocket(
            m_Fd,
            m_LocalSockAddr,
            NonBlocking()
            )
         );
}

boost::shared_ptr<CommunicationSocket> SocketImplBase::Accept()
{
   while(true)
   {
      sockaddr_in addr_in = {};
      socklen_t len = sizeof addr_in;

      errno = 0;
      int fd = accept(m_Fd, (sockaddr*)&addr_in, &len);
      int lerrno = errno;

      if(fd == -1)
      {
         if(lerrno == EINTR)
            continue;

         ThrowError("accept failed", lerrno);
      }

      return boost::shared_ptr<CommunicationSocket>(
            new CommunicationSocket(
               fd,
               addr_in,
               NonBlocking()
               )
            );
   }

   throw 1;
}

int SocketImplBase::SocketFD() const
{
   return m_Fd;
}
