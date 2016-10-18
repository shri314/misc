#include "Base.h"

Base::Base(int x)
   : m_x(x)
{
}

Base::Base()
   : m_x(-1)
{
}

void Base::SetX(int x) { m_x = x; }

int Base::GetX() const { return m_x; }

