#include "Derived2.h"

#include <iostream>

Derived2::Derived2(int x, int y)
: Base(x)
, m_y(y)
{
}


Derived2::Derived2()
: Base(0), m_y(0)
{
}


void Derived2::foo()
{
   std::cout << "Dervied2: [" << GetX() << ", " << m_y << "]" << std::endl;
}

//#include <boost/serialization/export.hpp>
//BOOST_CLASS_EXPORT_GUID(Derived2, "Derived2")
