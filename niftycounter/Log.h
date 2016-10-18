#pragma once
#include <string>
#include <iostream>

#include "NiftyCounter.h"

class Log
{
   public:
      void log( const std::string& s )
      {
         std::cout << m_prefix << " - log - " + s << "\n";
      }

      inline static Log& Instance();

   private:
     friend class NiftyCounter<Log>;

     Log(std::string prefix)
        : m_prefix(prefix)
     {
     }

   private:
     std::string m_prefix;
};

static NiftyCounter<Log> nf;

Log& Log::Instance()
{
   // void* x = NiftyCounter<Log>::impl::raw_ptr();
   // bool  x = nf.is_initialized();

   return nf.construct_once_get(std::string("debug"));
}
