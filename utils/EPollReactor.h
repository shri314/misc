#ifndef EPollReactor_h
#define EPollReactor_h

#include <sys/epoll.h>
#include <cerrno>
#include <vector>
#include <stdexcept>

class EventHandler
{
public:
   virtual void HandleSend() = 0;
   virtual void HandleRecv() = 0;
   virtual int CheckVisits() { return 1; }
   virtual ~EventHandler() { }
};

class EPollReactor
{
public:
   explicit EPollReactor(int size = 1024)
      : m_PollFd( epoll_create(size) )
   {
      int lerrno = errno;
      if(m_PollFd == -1)
         ThrowError("epoll_create", lerrno);
   }

   enum
   {
      REGISTER_FOR_READ = 1,
      REGISTER_FOR_WRITE = 2
   };

   void HandleEvents(int timeout = 1000 /*milliseconds*/) 
   {
      while(true)
      {
         errno = 0;
         int ret = epoll_wait(m_PollFd, &m_PollEvents[0], m_PollEvents.size(), timeout);
         if(ret == -1)
         {
            int lerrno = errno;
            if(lerrno == EINTR)
               continue;

            ThrowError("epoll_wait", lerrno);
         }

         break;
      }

      for(std::vector<epoll_event>::size_type i = 0; i < m_PollEvents.size(); ++i)
      {
         uint32_t GotEvents = m_PollEvents[i].events;
         EventHandler* ptr = (EventHandler*)m_RequEvents[i].data.ptr;

         if(ptr && ptr->CheckVisits())
         {
            uint32_t ReqEvents = m_RequEvents[i].events;
            ptr = (EventHandler*)m_RequEvents[i].data.ptr;

            if(ptr && GotEvents)
            {
               if((GotEvents & EPOLLIN) && (ReqEvents & EPOLLIN))
                  ptr->HandleRecv();
               if((GotEvents & EPOLLOUT) && (ReqEvents & EPOLLOUT))
                  ptr->HandleSend();

               if((GotEvents & (EPOLLERR|EPOLLHUP)))
               {
                  if(ReqEvents & EPOLLIN)
                     ptr->HandleRecv();
                  else if(ReqEvents & EPOLLOUT)
                     ptr->HandleSend();
               }
            }
         }
      }
   }

   void Register(int fd, int op, EventHandler* ptr)
   {
      if(!op) return;
      if(fd < 0) return;

      uint32_t Events = 0;
      Events |= ((op & REGISTER_FOR_READ) ? EPOLLIN : 0);
      Events |= ((op & REGISTER_FOR_WRITE) ? EPOLLOUT : 0);

      RegisterEvents(fd, Events, ptr);
   }

   void Unregister(int fd, int op)
   {
      if(!op) return;
      if(fd < 0 && (std::vector<epoll_event>::size_type)fd > m_PollEvents.size()) return;

      uint32_t Events = 0;
      Events &= ((op & REGISTER_FOR_READ) ? ~EPOLLIN : ~0);
      Events &= ((op & REGISTER_FOR_WRITE) ? ~EPOLLOUT : ~0);

      RegisterEvents(fd, Events, 0);
   }

   ~EPollReactor()
   {
      close(m_PollFd);
   }

private:
   static void ThrowError(const std::string& context, int lerrno)
   {
      throw std::runtime_error(std::string("error during ") + context + " - " + strerror(lerrno));
   }

   void RegisterEvents(int fd, uint32_t Events, EventHandler* ptr)
   {
      if((std::vector<epoll_event>::size_type)fd >= m_PollEvents.size())
      {
         m_PollEvents.resize(fd + 1);
         m_RequEvents.resize(fd + 1);
      }

      uint32_t OldEvents = m_RequEvents[fd].events;

      int ctl_op = 0;
      if( Events != 0 && OldEvents == 0 ) ctl_op = EPOLL_CTL_ADD;
      if( Events != 0 && OldEvents != 0 ) ctl_op = EPOLL_CTL_MOD;
      if( Events == 0 && OldEvents != 0 ) ctl_op = EPOLL_CTL_DEL;
      if( Events == 0 && OldEvents == 0 ) ctl_op = 0;

      if(ctl_op)
      {
         m_RequEvents[fd].events = Events;
         m_PollEvents[fd].events = Events;

         if(ctl_op == EPOLL_CTL_DEL)
            m_RequEvents[fd].data.ptr = 0;
         else if(ptr)
            m_RequEvents[fd].data.ptr = ptr;

         errno = 0;
         int Ret = epoll_ctl(m_PollFd, ctl_op, fd, &m_PollEvents[fd]);
         if(Ret == -1)
         {
            int lerrno = errno;
            ThrowError("epoll_ctl", lerrno);
         }
      }
   }

private:
   int m_PollFd;
   std::vector<epoll_event> m_PollEvents;
   std::vector<epoll_event> m_RequEvents;
};

#endif
