// vi:sw=3:expandtab:ts=8:shiftround

#pragma once

#include <utility>
#include <new>

template<class T, int Number = 0>
class NiftyCounter
{
   public:
      friend T;

      NiftyCounter()
      {
         ++impl::count;
      }

      ~NiftyCounter()
      {
         --impl::count;

         if(impl::count == 0)
            destruct_once();
      }

   private:
      template<class... U>
      void construct_once(U&& ... u)
      {
         if(!impl::init_flag)
         {
            new(impl::raw_ptr()) T(std::forward<U>(u)...);

            impl::init_flag = true;
         }
      }

      void destruct_once()
      {
         if(impl::init_flag)
         {
            impl::init_flag = false;

            impl::raw_ptr()->~T();
         }
      }

      bool is_initialized() const
      {
         return impl::init_flag;
      }

      template<class... U>
      T& construct_once_get(U&& ... u)
      {
         construct_once(std::forward<T>(u)...);

         return impl::raw_ref();
      }

      template<class... U>
      T& reconstruct_once_get(U&& ... u)
      {
         destruct_once();

         construct_once(std::forward<T>(u)...);

         return impl::raw_ref();
      }

   private:
      class impl
      {
         friend class NiftyCounter;

      private:
         static T& raw_ref()
         {
            return reinterpret_cast<T&>(storage);
         }

         static T* raw_ptr()
         {
            return &(raw_ref());
         }

         static int count;
         static bool init_flag;
         static typename std::aligned_storage<sizeof(T), alignof(T)>::type storage;
      };
};

template<class T, int Number>
int NiftyCounter<T, Number>::impl::count;

template<class T, int Number>
bool NiftyCounter<T, Number>::impl::init_flag;

template<class T, int Number>
typename std::aligned_storage<sizeof(T), alignof(T)>::type NiftyCounter<T, Number>::impl::storage;
