#include "EPollReactor.h"
#include "ConnectingSocket.h"
#include "CommunicationSocket.h"
#include <iostream>

class Sample : public EventHandler
{
public:
   Sample(EPollReactor* R)
      : m_R(R)
      , m_ConnS( new ConnectingSocket("192.168.29.194:25", true) )
   {
      m_R->Register( m_ConnS->SocketFD(), EPollReactor::REGISTER_FOR_READ, this);
   }

   ~Sample()
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
         m_ConnS.reset();
      }

      std::vector<char> IBuffer;
      IBuffer.resize(100);

      m_CommS->Read(IBuffer);

      std::cout << "Received:" + std::string(IBuffer.begin(), IBuffer.end()) << std::endl;
      exit(1);
   }

   void Reset()
   {
      if(m_ConnS)
         m_R->Unregister( m_ConnS->SocketFD(), EPollReactor::REGISTER_FOR_READ|EPollReactor::REGISTER_FOR_WRITE);
      if(m_CommS)
         m_R->Unregister( m_CommS->SocketFD(), EPollReactor::REGISTER_FOR_READ|EPollReactor::REGISTER_FOR_WRITE);
   }

private:
   EPollReactor* m_R;
   boost::shared_ptr<ConnectingSocket> m_ConnS;
   boost::shared_ptr<CommunicationSocket> m_CommS;
};

#include "ConnectingSocket.h"
#include "ListeningSocket.h"
#include "CommunicationSocket.h"

int main(int argc,char *argv[])
{
   EPollReactor R;

   Sample s(&R);

   while(true)
   {
      R.HandleEvents(1);
      sleep(1);
   }

   return 0;
}

