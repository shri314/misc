#ifndef DERIVED2_H__
#define DERIVED2_H__

#include "Base.h"

// #include <boost/serialization/export.hpp>

class Derived2 : public Base
{
   public:
      explicit Derived2(int x, int y);

   private:
      Derived2();

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

// BOOST_CLASS_EXPORT_GUID(Derived2, "Derived2")

#endif
