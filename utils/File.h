#ifndef File_h
#define File_h

#include <string>
#include <vector>
#include <exception>

class File
{
public:
   class Exception : public std::exception
   {
   public:
      Exception(const std::string& context, int lerrno, const std::string& error = "");
      const char* what() const throw() { return m_description.c_str(); }
      ~Exception() throw() { }
   private:
      std::string m_description;
   };

   enum ModeFlags
   {
      RDONLY = 1,
      WRONLY = 2,
      APPEND = 4,
      TRUNC = 8,
      EXCL = 16,
      RDWR = RDONLY | WRONLY
   };

   friend ModeFlags operator|(ModeFlags lhs, ModeFlags rhs)
   {
      return (ModeFlags) ((int)lhs|(int)rhs);
   }

   struct Offset
   {
      enum Flags
      {
         SET,
         CUR,
         END
      };
   };

   typedef int size_type;

public:
   File(const std::string& filename, ModeFlags flags);
   File(const std::string& filename, ModeFlags flags, int permissions); // create
   ~File();

   int RawRead(void* IBuff, int ReqCount);
   void RawWrite(const void* OBuff, int ReqCount);

   void Resize(size_type NewLength);
   size_type Size();
   void Seek(size_type Offset, Offset::Flags flags = Offset::SET);

   template<class T>
   void Read(T& IBuff)
   {
      int x = RawRead((void*)&IBuff, sizeof IBuff);
      if(x != sizeof IBuff)
         throw Exception("read", 0, "incomplete");
   }

   template<class T>
   T Read()
   {
      T IBuff;
      Read(IBuff);
      return IBuff;
   }

   template<size_t N>
   void Read(char (&IBuff)[N])
   {
      int x = RawRead((void*)&IBuff[0], N);
      if(x != N)
         throw Exception("read", 0, "incomplete");
   }

   template<class T>
   void Read(T& IBuff, int ReqCount)
   {
      IBuff.resize( ReqCount );
      IBuff.resize( RawRead((void*)&IBuff[0], IBuff.size()) );

      return IBuff.size();
   }

   template<class T>
   T Read(int ReqCount)
   {
      T IBuff;
      IBuff.resize( ReqCount );
      IBuff.resize( RawRead((void*)&IBuff[0], IBuff.size()) );

      return IBuff;
   }

   template<class T>
   void Write(const T& OBuff)
   {
      RawWrite((const void*)&OBuff, sizeof OBuff);
   }

   template<size_t N>
   void Write(const char (&OBuff)[N])
   {
      RawWrite((const void*)&OBuff[0], N);
   }

   void Write(const std::string& OBuff)
   {
      RawWrite((const void*)&OBuff[0], OBuff.size());
   }

   void Write(const std::vector<char>& OBuff)
   {
      RawWrite((const void*)&OBuff[0], OBuff.size());
   }

   int Descriptor() const { return m_fd; }

   std::string Name() const { return m_name; }

private:
   void Open(int o_flags, int permissions);
   void Close();
   static int OFlags(ModeFlags flags);

private:
   int m_fd;
   std::string m_name;
};

#endif
