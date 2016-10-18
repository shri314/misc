#ifndef SERIALIZER_H__
#define SERIALIZER_H__

#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/serialization/shared_ptr.hpp> 

#include <sstream>
#include <string>

template<class T>
std::string Serialize( const T& t )
{
   std::ostringstream oss;
   boost::archive::binary_oarchive oa(oss);
   oa << t;

   return oss.str();
}

template<class T>
T Deserialize( const std::string& buff )
{
   std::istringstream iss(buff);
   boost::archive::binary_iarchive ia(iss);

   T t;
   ia >> t;

   return t;
}

#endif
