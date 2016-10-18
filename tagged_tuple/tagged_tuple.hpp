#ifndef tagged_tuple_hpp__
#define tagged_tuple_hpp__

#include <tuple>
#include <type_traits>

/*
 * @author Shriram V (shri314@yahoo.com)
 */

namespace boost
{
   namespace tagged_tuple_impl
   {
      template<class... Args>
      struct typelist
      {
         template<class T>
         using push_front_t = typelist<T, Args...>;

         using tuple_t = std::tuple<Args...>;
      };

      /////////////////////////

      template<class>
      struct select_even;

      template<class>
      struct select_odd;

      template<class First, class... Remaining>
      struct select_even< typelist<First, Remaining...> >
      {
         typedef typename select_odd< typelist<Remaining...> >::type ::template push_front_t<First> type;
      };

      template<>
      struct select_even<typelist<>>
      {
         typedef typelist<> type;
      };

      template<class First, class... Remaining>
      struct select_odd< typelist<First, Remaining...> >
      {
         typedef typename select_even< typelist<Remaining...> >::type type;
      };

      template<>
      struct select_odd<typelist<>>
      {
         typedef typelist<> type;
      };

      /////////////////////////

      template<class Tag, class TL>
      struct index_of;

      template<class Tag, class T, class... Args>
      struct index_of<Tag, typelist<T, Args...>>
      {
         static const int value = std::is_same<Tag, T>::value ? 0 : 1 + index_of<Tag, typelist<Args...> >::value;
      };

      template<class Tag>
      struct index_of<Tag, typelist<>>
      {
         static const int value = 0;
      };

      /////////////////////////

      template<class... Args>
      using to_types_t = typename select_odd< typelist<Args...> >::type;

      template<class... Args>
      using to_tags_t = typename select_even< typelist<Args...> >::type;

      template<class... Args>
      using to_tuple_t = typename to_types_t<Args...>::tuple_t;
   }

   inline namespace
   {
      using namespace tagged_tuple_impl;

      template<class... Args>
      struct tagged_tuple : public to_tuple_t<Args...>
      {
         using typename to_tuple_t<Args...>::tuple;

         template<class Tag>
         auto get() -> decltype(std::get< index_of<Tag, to_tags_t<Args...> >::value >(*this))
         {
            return std::get< index_of<Tag, to_tags_t<Args...> >::value >(*this);
         }

         template<int Tag>
         auto get() -> decltype(std::get< Tag >(*this))
         {
            return std::get< Tag >(*this);
         }
      };
   }
}

#endif
