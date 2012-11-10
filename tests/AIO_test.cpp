#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

const char* SAMPLE_FILE = "/tmp/posix_aio_sample.txt";

void ConventionalIO()
{
   std::cout << "Conventional IO" << std::endl;
   std::cout << "===============" << std::endl;

   int fd = open(SAMPLE_FILE, O_RDONLY);

   while(true)
   {
      char buf[30] = {};
      int x = read(fd, buf, sizeof buf - 1);
      if(x == 0)
         break;
      if(x == -1)
         break;
      if(x > 0)
      {
         std::cout << buf << std::flush;
      }
   }
   
   close(fd);
}

#include "AIOHandler.h"

void SampleAIO()
{
   std::cout << "Sample AIO" << std::endl;
   std::cout << "==========" << std::endl;

   AIOHandler aio;

   int fd = open(SAMPLE_FILE, O_RDONLY);

   off_t Offset = 0;
   while(true)
   {
      std::vector<char> IBuffer;
      AIOReader reader(fd, 30, Offset, IBuffer);
      if(IBuffer.empty())
         break;

      Offset += IBuffer.size();

      while(reader.InProgress());

      std::cout << std::string(IBuffer.begin(), IBuffer.end()) << std::flush;
   }
}

void CreatSampleFile()
{
   int fd = open(SAMPLE_FILE, O_WRONLY|O_CREAT, 0644);

   std::string s = 
"This is one line\n"
"This is another line\n"
"This is third line\n"
"This is yet another fourth line that seems to be quite long\n"
"This is yet another fifth line that is also quite long, but not terribly long\n";

   write(fd, s.data(), s.size());
   close(fd);
}

int main(int argc,char *argv[])
{
   CreatSampleFile();

   ConventionalIO();

   SampleAIO();

   return 0;
}

