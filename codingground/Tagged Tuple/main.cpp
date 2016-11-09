// g++ -std=c++14

#include "tagged_tuple.cpp"

#include <iostream>
#include <string>

using namespace std;

struct name;
struct age;

template<class TT>
void print(const TT& tt)
{
   // get<name>(tt) = "def";   // expected error modifying const
   
   cout << "{ name = " << get<name>(tt) << ", age = " << get<age>(tt) << " }\n";
}

int main()
{
   tagged_tuple<name, std::string, age, int> tt = std::make_tuple("abc", 23);
   
   print(tt);

   get<0>(tt) += "def";
   get<1>(tt) += 100;

   print(tt);
}