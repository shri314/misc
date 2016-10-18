#include "Base.h"
#include "Derived1.h"
#include "Derived2.h"
#include "Serializer.h"

#include <boost/shared_ptr.hpp>
#include <fstream>
#include <string>

// BOOST_CLASS_EXPORT_GUID(Derived1, "Derived1")
// BOOST_CLASS_EXPORT_GUID(Derived2, "Derived2")

int main(int argc, const char* argv[]) try
{
   --argc;
   ++argv;

   std::string option = argc > 0 ? *argv : "";

   if(option == "" || option == "--write")
   {
      boost::shared_ptr<Base> b( new Derived1(10, 15) );
      b->foo();

      std::string buffer = Serialize( b );

      std::ofstream ofs("filename");
      ofs << buffer;
   }

   if(option == "" || option == "--read")
   {
      std::ifstream ifs("filename");
      std::string buffer((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

      boost::shared_ptr<Base> b = Deserialize< boost::shared_ptr<Base> > ( buffer );
      b->foo();
   }
}
catch(const std::exception& e)
{
   std::cout << typeid(e).name() << e.what() << std::endl;
}
