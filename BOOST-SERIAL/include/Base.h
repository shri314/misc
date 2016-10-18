#ifndef BASE_H__
#define BASE_H__

#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>

class Base
{
   public:
      virtual void foo() = 0;

      explicit Base(int x);

      void SetX(int x);
      
      virtual ~Base() { }

   protected:
      int GetX() const;

   private:
      Base();

      friend class boost::serialization::access;
      template<class Archive>
         void serialize(Archive & ar, const unsigned int version)
      {
         ar & m_x;
      }

   private:
      int m_x;
};

BOOST_SERIALIZATION_ASSUME_ABSTRACT(Base)

#endif
