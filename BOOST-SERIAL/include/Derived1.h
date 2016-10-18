#ifndef DERIVED1_H__
#define DERIVED1_H__

#include "Base.h"

// #include <boost/serialization/export.hpp>

class Derived1 : public Base
{
   public:
      explicit Derived1(int x, int y);

   private:
      Derived1();

      void foo();

      friend class boost::serialization::access;
      template<class Archive>
         void serialize(Archive & ar, const unsigned int version)
      {
         ar & boost::serialization::base_object<Base>(*this);
         ar & m_y;
      }

   private:
      int m_y;
};

// BOOST_CLASS_EXPORT_GUID(Derived1, "Derived1")

#endif
