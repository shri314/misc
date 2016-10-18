#include <sys/types.h>
#include <unistd.h>

struct Entry
{
   int id;
   int salary;
   int dob;

   Entry(int id, int salary, int dob)
      : id(id)
      , salary(salary)
      , dob(dob)
   {
   }
};

//////////////////////////////////////////////////////////////

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/mem_fun.hpp>

using namespace ::boost;
using namespace ::boost::multi_index;

// define a multiply indexed set with indices by id and salary
typedef multi_index_container<
/**/ Entry,
/**/ indexed_by<
/**/    ordered_unique<
/**/       member<Entry,int,&Entry::id>
/**/    >,
/**/    ordered_non_unique<
/**/       member<Entry,int,&Entry::salary>
/**/    >
/**/ >
> container;

//////////////////////////////////////////////////////////////

int main(int argc,char *argv[])
{
   container c;

   c.insert( Entry(10, 10, 10) );
   c.insert( Entry(11, 10, 10) );

   return 0;
}
