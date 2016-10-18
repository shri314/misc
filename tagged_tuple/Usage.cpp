#include "tagged_tuple.hpp"

struct name;
struct age;

#include <string>
#include <iostream>

int main()
{
   boost::tagged_tuple<name, std::string, age, int> t1 = std::make_tuple("abc", 12);

   t1.get<0>() += "def";
   t1.get<1>() += 24;

   t1.get<name>() += "ghi";
   t1.get<age>() += 24;

   std::cout << t1.get<name>() << std::endl;
   std::cout << t1.get<age>() << std::endl;
}
