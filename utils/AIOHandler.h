#ifndef AIOHandler_h
#define AIOHandler_h

#include <aio.h>

#include <cassert>
#include <cerrno>

#include <vector>
#include <stdexcept>

class AIOOperator
{
public:
   virtual void Event() = 0;
   virtual ~AIOOperator() { }
};

class AIOHandler
{
public:
   AIOHandler()
      : m_OldSigAct( EmptySigAct() )
   {
      struct sigaction sig_act;
      sigemptyset(&sig_act.sa_mask);
      sig_act.sa_flags = SA_SIGINFO;
      sig_act.sa_sigaction = &AIOHandler::AIOCompletionHandler;

      sigaction(SIGIO, &sig_act, &m_OldSigAct);
   }

   static void AIOCompletionHandler(int SigNo, siginfo_t* info, void* Context)
   {
      if(info->si_signo == SIGIO)
      {
         AIOOperator* OP = (AIOOperator*)info->si_value.sival_ptr;

         OP->Event();
      }
   }

   ~AIOHandler()
   {
      sigaction(SIGIO, &m_OldSigAct, NULL);
   }

private:
      static const struct sigaction& EmptySigAct()
      {
         static struct sigaction empty = {};
         return empty;
      }

private:
   struct sigaction m_OldSigAct;
};


class AIOReader : public AIOOperator
{
   public:
      AIOReader(int fd, unsigned int Count, off_t Offset, std::vector<char>& IBuffer)
         : m_Handle( EmptyHandle() )
         , m_errno( 0 )
         , m_InProgress( true )
         , m_IBuffer( IBuffer )
      {
         Read(fd, Count, Offset);
      }

      ~AIOReader()
      {
         aio_cancel(m_Handle.aio_fildes, &m_Handle);

         assert(!InProgress());
      }

      bool InProgress() const
      {
         return m_InProgress;
      }

      void Check() const
      {
         if(!InProgress())
         {
            throw std::runtime_error( std::string("error during aio_read - ") + strerror(m_errno) );
         }
      }

   private:
      static const struct aiocb& EmptyHandle()
      {
         static struct aiocb empty = {};
         return empty;
      }

      void Read(int fd, unsigned int Count, off_t Offset = 0)
      {
         m_IBuffer.resize(Count);

         m_Handle.aio_fildes = fd;
         m_Handle.aio_offset = Offset;
         m_Handle.aio_buf = &m_IBuffer[0];
         m_Handle.aio_nbytes = m_IBuffer.size();

         m_Handle.aio_sigevent.sigev_notify = SIGEV_SIGNAL;
         m_Handle.aio_sigevent.sigev_signo = SIGIO;
         m_Handle.aio_sigevent.sigev_value.sival_ptr = (void*)this;

         while(true)
         {
            errno = 0;
            if(-1 == aio_read(&m_Handle))
            {
               if(errno == EINTR)
                  continue;

               m_InProgress = false;
               m_errno = errno;

               Check();
            }

            break;
         }
      }

      void Event()
      {
         int lerrno = aio_error(&m_Handle);

         if( lerrno == 0 )
         {
            ssize_t Ret = aio_return(&m_Handle);

            if(Ret >= 0 && (unsigned)Ret < m_IBuffer.size())
               m_IBuffer.resize(Ret);

            m_InProgress = false;
         }
         else if( lerrno != EINPROGRESS )
         {
            m_errno = lerrno;
            m_InProgress = false;
         }
      }

   private:
      struct aiocb m_Handle;
      int m_errno;
      bool m_InProgress;
      std::vector<char>& m_IBuffer;
};

#endif
