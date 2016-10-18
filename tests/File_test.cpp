#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

#include "utils/File.h"
#include <ostream>

BOOST_AUTO_TEST_CASE( TextFileRW )
{
   std::string writes;

   {
      File f("/tmp/test.txt", File::Mode::RDWR|File::Mode::TRUNC, 0644);

      f.Write(std::string("papa\n"));
      f.Write(std::string("papaji\n"));

      writes += "papa\n";
      writes += "papaji\n";
   }

   {
      File f("/tmp/test.txt", File::Mode::RDONLY);

      std::string reads;
      while(true)
      {
         std::string s = f.Read<std::string>(5);
         reads += s;

         if(s.empty())
            break;
      }

      BOOST_CHECK_EQUAL(reads, writes);
   }
}


struct RawEntry
{
   int v1;
   int v2;

   explicit RawEntry(int v1 = 0, int v2 = 0)
      : v1(v1)
      , v2(v2)
   {
   }

   friend bool operator==(const RawEntry& lhs, const RawEntry& rhs)
   {
      return
         lhs.v1 == rhs.v1 &&
         lhs.v2 == rhs.v2;
   }

   friend std::ostream& operator<<(std::ostream& os, const RawEntry& rhs)
   {
      return os << "v1 = " << rhs.v1 << ", v2 = " << rhs.v2;
   }
};

BOOST_AUTO_TEST_CASE( BinaryFileRW )
{
   {
      File f("/tmp/test.txt", File::Mode::RDWR|File::Mode::TRUNC, 0644);

      f.Resize( sizeof(RawEntry) * 1000 );

      f.Write( RawEntry(10, 11) );
      f.Write( RawEntry(11, 12) );
      f.Write( RawEntry(12, 13) );
      f.Write( RawEntry(13, 14) );
   }
   {
      File f("/tmp/test.txt", File::Mode::RDONLY);

      f.Seek(0);
      BOOST_CHECK( RawEntry(10, 11) == f.Read<RawEntry>() );
      BOOST_CHECK( RawEntry(11, 12) == f.Read<RawEntry>() );
      BOOST_CHECK( RawEntry(12, 13) == f.Read<RawEntry>() );
      BOOST_CHECK( RawEntry(13, 14) == f.Read<RawEntry>() );
      f.Seek(0);
      BOOST_CHECK( RawEntry(10, 11) == f.Read<RawEntry>() );
   }
}
