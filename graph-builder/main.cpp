#include "graph.h"
#include <string>
#include <iostream>

int main()
{
   graph<std::string, int> g;

   g.add_edge("a", "b", 1);
   g.add_edge("a", "e", 1);

   g.traverse_by_depth(
      [](auto v) {
         std::cout << "vertex: " << v << '\n';
      },
      [](auto a, auto b, auto w) {
         std::cout << a << "-(" << w << ")->" << b << '\n';
      }
   );
}
