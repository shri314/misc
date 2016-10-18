#include <list>
#include <algorithm>

using namespace std;

const std::list<int> push_front(const int v, const std::list<int> x)
{
   if(x.size())
   {
      std::list<int> L( x.begin(), x.end() );
      L.push_front( v );

      return L;
   }
   else
   {
      return std::list<int>{ v };
   }
}

const std::list<int> select_even(const std::list<int> i);
const std::list<int> select_odd(const std::list<int> i);

const std::list<int> select_odd(const std::list<int> i)
{
   if(i.size())
   {
      const std::list<int> n(++(i.begin()), i.end());

      const std::list<int> r = select_even(n);

      return r;
   }
   else
   {
      return std::list<int>();
   }
}

const std::list<int> select_even(const std::list<int> i)
{
   if(i.size())
   {
      const std::list<int> n(++(i.begin()), i.end());

      const std::list<int> r = select_odd(n);

      return push_front(i.front(), r);
   }
   else
   {
      return std::list<int>();
   }
}

#include <iostream>

int main()
{
   std::list<int> i = { 10, 111, 332, 4343 };

   for(auto v : i)
   {
      std::cout << v << std::endl;
   }

   std::cout << std::endl;

   std::list<int> j = select_even(i);

   for(auto v : j)
   {
      std::cout << v << std::endl;
   }
}
