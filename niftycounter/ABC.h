#pragma once
#include "Log.h"

class ABC
{
public:
   void foo()
   {
      Log::Instance().log("in ABC::foo");

      ++m_x;
   }

   ~ABC()
   {
      Log::Instance().log("in ABC::~ABC");
   }

   static ABC* Instance()
   {
      return &abc;
   }

private:
   ABC() = default;

   int m_x = 0;

   static ABC abc;
};
