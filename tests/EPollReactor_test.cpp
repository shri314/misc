#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "utils/EPollReactor.h"
#include "utils/ListeningSocket.h"
#include "utils/ConnectingSocket.h"
#include "utils/CommunicationSocket.h"

class TestClient : public EventHandler
{
public:
   TestClient(EPollReactor* R)
      : m_R(R)
      , m_ConnS( new ConnectingSocket("127.0.0.1:29999", true) )
   {
      m_R->Register( m_ConnS->SocketFD(), EPollReactor::REGISTER_FOR_READ, this );
   }

   ~TestClient()
   {
      Reset();
   }

private:
   void HandleSend()
   {
   }

   void HandleRecv()
   {
      if(!m_CommS)
      {
         m_CommS = m_ConnS->Talk();

         m_IBuffer.clear();
         m_ExpectedIBuffer = "Hello World";

         m_R->Unregister( m_ConnS->SocketFD(), EPollReactor::REGISTER_FOR_READ );
         m_R->Register( m_CommS->SocketFD(), EPollReactor::REGISTER_FOR_READ, this );

         m_ConnS.reset();
      }

      if( std::vector<char>::size_type s = m_ExpectedIBuffer.size() - m_IBuffer.size() )
      {
         std::vector<char> IBuffer(s);
         m_CommS->Read(IBuffer);

         m_IBuffer.append( IBuffer.begin(), IBuffer.end() );
      }

      if( m_IBuffer.size() >= m_ExpectedIBuffer.size() )
      {
         m_R->Unregister( m_ConnS->SocketFD(), EPollReactor::REGISTER_FOR_READ );

         BOOST_CHECK_EQUAL( m_IBuffer, m_ExpectedIBuffer );
      }
   }

   void Reset()
   {
      if(m_ConnS)
         m_R->Unregister( m_ConnS->SocketFD(), EPollReactor::REGISTER_FOR_READ|EPollReactor::REGISTER_FOR_WRITE );
      if(m_CommS)
         m_R->Unregister( m_CommS->SocketFD(), EPollReactor::REGISTER_FOR_READ|EPollReactor::REGISTER_FOR_WRITE );
   }

private:
   EPollReactor* m_R;
   boost::shared_ptr<ConnectingSocket> m_ConnS;
   boost::shared_ptr<CommunicationSocket> m_CommS;
   std::string m_IBuffer;
   std::string m_ExpectedIBuffer;
};


class TestServer : public EventHandler
{
public:
   TestServer(EPollReactor* R)
      : m_R(R)
      , m_LisnS( new ListeningSocket("127.0.0.1:29999", true) )
   {
      m_R->Register( m_LisnS->SocketFD(), EPollReactor::REGISTER_FOR_READ, this );
   }

   ~TestServer()
   {
      Reset();
   }

private:
   void HandleSend()
   {
      if(!m_OBuffer.empty())
      {
         m_CommS->Write( m_OBuffer );
      }

      if(!m_OBuffer.empty())
      {
         m_R->Unregister( m_CommS->SocketFD(), EPollReactor::REGISTER_FOR_WRITE );
      }
   }

   void HandleRecv()
   {
      if(!m_CommS)
      {
         m_CommS = m_LisnS->Accept();

         std::string s = "Hello World";
         m_OBuffer.clear();
         m_OBuffer.insert( m_OBuffer.end(), s.begin(), s.end() );

         m_R->Unregister( m_LisnS->SocketFD(), EPollReactor::REGISTER_FOR_READ );
         m_R->Register( m_CommS->SocketFD(), EPollReactor::REGISTER_FOR_WRITE, this );

         m_CommS.reset();
      }
   }

   void Reset()
   {
      if(m_LisnS)
         m_R->Unregister( m_LisnS->SocketFD(), EPollReactor::REGISTER_FOR_READ|EPollReactor::REGISTER_FOR_WRITE );
      if(m_CommS)
         m_R->Unregister( m_CommS->SocketFD(), EPollReactor::REGISTER_FOR_READ|EPollReactor::REGISTER_FOR_WRITE );
   }

private:
   EPollReactor* m_R;
   boost::shared_ptr<ListeningSocket> m_LisnS;
   boost::shared_ptr<CommunicationSocket> m_CommS;
   std::vector<char> m_OBuffer;
};

BOOST_AUTO_TEST_CASE( BasicSample )
{
   EPollReactor R;

   int x = fork();

   BOOST_ASSERT( x >= 0 );

   if(x > 0)
   {
      // parent
      TestServer s(&R);

      sleep(1);

      while(R.Size())
      {
         R.HandleEvents(1);
      }
   }
   else
   {
      sleep(1);

      // child
      TestClient s(&R);

      while(R.Size())
      {
         R.HandleEvents(1);
      }

      exit(0);
   }
}

