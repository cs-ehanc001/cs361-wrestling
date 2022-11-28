#ifndef SUPPLEMENTARIES_TYPE_LIST_HPP
#define SUPPLEMENTARIES_TYPE_LIST_HPP

// Still a work in progress

#include <type_traits>

#include "metaprogramming.hpp"

namespace supl {

///////////////////////////////////////////// Type List

template <typename... Pack>
struct type_list {};

///////////////////////////////////////////// contains

template <typename T, typename LIST>
struct contains_type;

template <typename T, template <typename...> typename LIST,
          typename... Pack>
struct contains_type<T, LIST<Pack...>>
    : ::supl::is_type_in_pack<T, Pack...> {};

template <typename T, typename LIST>
constexpr inline bool contains_type_v =
    ::supl::contains_type<T, LIST>::value;

///////////////////////////////////////////// type_list_size

template <typename LIST>
struct type_list_size;

template <template <typename...> typename LIST, typename... Pack>
struct type_list_size<LIST<Pack...>>
    : ::supl::index_constant<sizeof...(Pack)> {};

template <typename LIST>
constexpr inline std::size_t type_list_size_v =
    ::supl::type_list_size<LIST>::value;

///////////////////////////////////////////// type_at_index

template <std::size_t Idx, typename LIST, std::size_t Current = 0>
struct type_at_index;

template <std::size_t Idx, template <typename...> typename LIST,
          typename T, typename... Pack, std::size_t Current>
struct type_at_index<Idx, LIST<T, Pack...>, Current>
    : std::conditional_t<
          Idx == Current, ::supl::type_identity<T>,
          ::supl::type_at_index<Idx, LIST<Pack...>, Current + 1>> {
  static_assert(Idx - Current <= sizeof...(Pack), "Index out of bounds");
};

template <std::size_t Idx, typename LIST, std::size_t Current = 0>
using type_at_index_t = typename ::supl::type_at_index<Idx, LIST>::type;

///////////////////////////////////////////// push_back

template <typename LIST, typename T>
struct push_back;

template <template <typename...> typename LIST, typename T,
          typename... Pack>
struct push_back<LIST<Pack...>, T>
    : ::supl::type_identity<LIST<Pack..., T>> {};

template <typename LIST, typename T>
using push_back_t = typename ::supl::push_back<LIST, T>::type;

///////////////////////////////////////////// push_front

template <typename LIST, typename T>
struct push_front;

template <template <typename...> typename LIST, typename... Pack,
          typename T>
struct push_front<LIST<Pack...>, T>
    : ::supl::type_identity<LIST<T, Pack...>> {};

template <typename LIST, typename T>
using push_front_t = typename ::supl::push_front<LIST, T>::type;

///////////////////////////////////////////// pop_back
/// oh boy...

///////////////////////////////////////////// pop_front
/// oh boy...

} // namespace supl

#endif
