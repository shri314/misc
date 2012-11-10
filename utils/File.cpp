#include "File.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <cerrno>
#include <stdexcept>

File::File(const std::string& filename, ModeFlags mode)
   : m_fd(-1)
   , m_name(filename)
{
   Open(OFlags(mode), 0644);
}

File::File(const std::string& filename, ModeFlags mode, int permissions)
   : m_fd(-1)
   , m_name(filename)
{
   Open(OFlags(mode) | O_CREAT, permissions);
}

File::~File() // nothrow
{
   Close();
}

void File::Open(int o_flags, int permissions)
{
   Close();

   while(true)
   {
      errno = 0;
      m_fd = open(m_name.c_str(), o_flags|O_LARGEFILE, permissions);
      if(m_fd == -1)
      {
         int lerrno = errno;
         if(lerrno == EINTR || lerrno == EWOULDBLOCK)
            continue;

         throw Exception("open", lerrno);
      }

      break;
   }
}

int File::RawRead(void* IBuff, int ReqCount)
{
   int Count = ReqCount;
   while(Count)
   {
      errno = 0;
      int ret = read(m_fd, IBuff, Count);
      if(ret == -1)
      {
         int lerrno = errno;
         if(lerrno == EINTR || lerrno == EAGAIN)
            continue;

         throw Exception("read", lerrno);
      }

      if(ret == 0)
         break;

      IBuff = (char*)IBuff + ret;
      Count -= ret;
   }

   return ReqCount - Count;
}

void File::RawWrite(const void* OBuff, int Count)
{
   while(Count)
   {
      errno = 0;
      int ret = write(m_fd, OBuff, Count);
      if(ret == -1)
      {
         int lerrno = errno;
         if(lerrno == EINTR || lerrno == EAGAIN)
            continue;

         throw Exception("write", lerrno);
      }

      OBuff = (const char*)OBuff + ret;
      Count -= ret;
   }
}

void File::Close() // nothrow
{
   while(m_fd != -1)
   {
      errno = 0;
      int x = close(m_fd);
      if(x == -1 && errno == EINTR)
         continue;

      m_fd = -1;
   }
}

void File::Resize(size_type NewLength)
{
   while(true)
   {
      errno = 0;
      int x = ftruncate(m_fd, NewLength);
      if(x == -1)
      {
         int lerrno = errno;
         if(lerrno == EINTR)
            continue;

         throw Exception("ftruncate", lerrno);
      }

      break;
   }
}

File::size_type File::Size()
{
   errno = 0;
   struct stat buf = {};
   int x = fstat(m_fd, &buf);
   if(x == -1)
   {
      int lerrno = errno;
      throw Exception("fstat", lerrno);
   }

   return buf.st_size;
}

void File::Seek(size_type offset, Offset::Flags flags)
{
   int whence = SEEK_SET;

   switch(flags)
   {
      case Offset::SET: whence = SEEK_SET; break;
      case Offset::CUR: whence = SEEK_CUR; break;
      case Offset::END: whence = SEEK_END; break;
   }

   errno = 0;
   int x = lseek(m_fd, offset, whence);
   if(x == -1)
   {
      int lerrno = errno;
      throw Exception("fstat", lerrno);
   }
}

File::Exception::Exception(const std::string& context, int lerrno, const std::string& error)
   : m_description( std::string("error occurred in - ") + context + (lerrno != 0 ? std::string(" - ") + strerror(lerrno) : "") + ( error.empty() ? error : ( " - " + error ) ) )
{
}

int File::OFlags(ModeFlags mode)
{
   int o_flags = 0;
   o_flags |= ((mode & RDWR) ? O_RDWR : 0);
   o_flags |= ((!(o_flags & O_RDWR) && (mode & RDONLY)) ? O_RDONLY : 0);
   o_flags |= ((!(o_flags & O_RDWR) && (mode & WRONLY)) ? O_WRONLY : 0);
   o_flags |= ((mode & APPEND) ? O_APPEND : 0);
   o_flags |= ((mode & TRUNC) ? O_TRUNC : 0);
   o_flags |= ((mode & EXCL) ? O_EXCL : 0);

   return o_flags;
}

