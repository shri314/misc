#include <utility>
#include <tuple>

namespace detail
{
   /** INDEX SEQUENCES **/

   template<std::size_t offset, std::size_t factor, typename ind_seq_t>
   struct seq;

   template<std::size_t offset, std::size_t factor, std::size_t... indices>
   struct seq< offset, factor, std::index_sequence<indices...> >
   {
      using type = std::index_sequence < offset + factor * indices... >;
   };

   template<std::size_t offset, std::size_t factor, std::size_t extent>
   using make_index_seq = typename seq< offset, factor, std::make_index_sequence<extent> >::type;


   /** FILTER TUPLE **/

   template<typename ind_seq_t, typename... types>
   struct filter_tuple;

   template<std::size_t... indices, typename... types>
   struct filter_tuple< std::index_sequence<indices...>, types... >
   {
      using type = std::tuple< std::tuple_element_t< indices, std::tuple<types...> >... >;
   };


   /** INDEX_OF **/

   template<typename T, typename... types>
   struct index_of;

   template<typename T, typename... remaining_types>
   struct index_of<T, T, remaining_types... > : std::integral_constant< std::size_t, 0 >
   {
   };

   template<typename T, typename U, typename... remaining_types>
   struct index_of<T, U, remaining_types... > : std::integral_constant < std::size_t, 1 + index_of<T, remaining_types... >::value >
   {
   };


   /** INDEX_IN_TUPLE **/

   template<typename T, typename tuple_t>
   struct index_in_tuple;

   template<typename T, typename... types>
   struct index_in_tuple< T, std::tuple<types...> > : index_of<T, types...>
   {
   };


   template<typename... types>
   using extract_key_types_tuple = typename filter_tuple < make_index_seq < 0, 2, sizeof...(types) / 2 >, types... >::type;

   template<typename... types>
   using extract_value_types_tuple = typename filter_tuple < make_index_seq < 1, 2, sizeof...(types) / 2 >, types... >::type;

}

/** TAGGED TUPLE **/

template<typename... types>
struct tagged_tuple : public detail::extract_value_types_tuple<types...>
{
};

/** TAGGED TUPLE ACCESSORS **/

template<typename key, typename... types>
auto get(tagged_tuple<types...>& tt) -> decltype(auto)
{
   return get< detail::index_in_tuple< key, detail::extract_key_types_tuple<types...> >::value >(tt);
}

template<typename key, typename... types>
auto get(tagged_tuple<types...>&& tt) -> decltype(auto)
{
   return get< detail::index_in_tuple< key, detail::extract_key_types_tuple<types...> >::value >(std::move(tt));
}


#include <iostream>
#include <string>

using namespace std;

int main()
{
   tagged_tuple<struct name, std::string, struct age, int> T;

   get<name>(T) = "abc";
   get<age>(T) = 23;

   cout << "{ name = " << get<name>(T) << ", age = " << get<age>(T) << " }\n";

   get<0>(T) += "def";
   get<1>(T) += 100;

   cout << "{ name = " << get<0>(T)    << ", age = " << get<1>(T)   << " }\n";
}
