#include "Derived1.h"

#include <iostream>

Derived1::Derived1(int x, int y)
: Base(x)
, m_y(y)
{
}


Derived1::Derived1()
: Base(0), m_y(0)
{
}


void Derived1::foo()
{
   std::cout << "Dervied1: [" << GetX() << ", " << m_y << "]" << std::endl;
}

//#include <boost/serialization/export.hpp>
//BOOST_CLASS_EXPORT_GUID(Derived1, "Derived1")
