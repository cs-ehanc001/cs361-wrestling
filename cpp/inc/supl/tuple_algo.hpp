#ifndef UTILS_TUPLE_ALGO_HPP
#define UTILS_TUPLE_ALGO_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "metaprogramming.hpp"

namespace supl {

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Not
 * intended to be called outside ::supl::for_each_in_tuple.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Function type.
 *
 * @tparam Inds Sequence of integers for template wizardry.
 *
 * @param tup Tuple to apply function to a member of.
 *
 * @param func Function to apply to a member of tuple.
 */
/* }}} */
template <typename Tuple, typename Func, std::size_t... Inds>
constexpr void
for_each_in_tuple_impl(Tuple&& tup, Func&& func,
                       std::index_sequence<Inds...>) noexcept
{
  (func(std::get<Inds>(tup)), ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple. Would not
 * have been possible without what I learned from this CppCon talk:
 * https://www.youtube.com/watch?v=15etE6WcvBY
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Visitor function type.
 *
 * @param tup Tuple to apply a visitor function to every element of.
 *
 * @param func Visitor function.
 */
/* }}} */
template <typename Tuple, typename Func>
constexpr void for_each_in_tuple(const Tuple& tup, Func&& func) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  ::supl::impl::for_each_in_tuple_impl(tup, std::forward<Func>(func), seq);
}

namespace impl {

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside ::supl::for_each_in_tuple.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Function type.
 *
 * @tparam Inds Sequence of integers for template wizardry.
 *
 * @param tup Tuple to apply function to a member of.
 *
 * @param func Function to apply to a member of tuple.
 *
 * @return Tuple of values mapped from `tup` through `func`.
 */
/* }}} */
template <typename Tuple, typename Func, std::size_t... Inds>
constexpr auto tuple_transform_impl(const Tuple& tup, Func&& func,
                                    std::index_sequence<Inds...>) noexcept
{
  return std::tuple {func(std::get<Inds>(tup))...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a visitor function to every member of a tuple,
 * and maps the returned values to a new tuple.
 * Not intended to be called outside ::supl::for_each_in_tuple.
 * Would not have been possible without what I learned from this CppCon talk:
 * https://www.youtube.com/watch?v=15etE6WcvBY
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Func Visitor function type.
 *
 * @param tup Tuple to apply a visitor function to every element of.
 *
 * @param func Visitor function.
 *
 * @return Tuple of values mapped from `tup` through `func`.
 */
/* }}} */
template <typename Tuple, typename Func>
[[nodiscard]] constexpr auto tuple_transform(const Tuple& tup,
                                             Func&& func) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::supl::impl::tuple_transform_impl(tup, std::forward<Func>(func),
                                            seq);
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_any_of_impl(const Tuple& tup, Pred&& pred,
                  std::index_sequence<Inds...>) noexcept -> bool
{
  static_assert(std::conjunction_v<std::is_invocable_r<
                    bool, Pred, decltype(std::get<Inds>(tup))>...>,
                "Predicate must be invocable returning a bool with every "
                "type in the tuple");

  return (pred(std::get<Inds>(tup)) || ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Determine if any elements of a tuple satisfy a unary predicate
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Pred Type of unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @param tup Input tuple
 *
 * @param pred Unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @return Returns `true` if `pred` applied to any element of `tup`
 * returns true. Returns `false` otherwise.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_any_of(const Tuple& tup,
                                          Pred&& pred) noexcept -> bool
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::supl::impl::tuple_any_of_impl(tup, std::forward<Pred>(pred),
                                         seq);
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_all_of_impl(const Tuple& tup, Pred&& pred,
                  std::index_sequence<Inds...>) noexcept -> bool
{
  static_assert(std::conjunction_v<std::is_invocable_r<
                    bool, Pred, decltype(std::get<Inds>(tup))>...>,
                "Predicate must be invocable returning a bool with every "
                "type in the tuple");

  return (pred(std::get<Inds>(tup)) && ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Determine if all elements of a tuple satisfy a unary predicate
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Pred Type of unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @param tup Input tuple
 *
 * @param pred Unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @return Returns `true` if `pred` applied to all elements of `tup`
 * return true. Returns `false` otherwise.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_all_of(const Tuple& tup,
                                          Pred&& pred) noexcept -> bool
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::supl::impl::tuple_all_of_impl(tup, std::forward<Pred>(pred),
                                         seq);
}

/* {{{ doc */
/**
 * @brief Determine if no elements of a tuple satisfy a unary predicate
 *
 * @tparam Tuple Input tuple type
 *
 * @tparam Pred Type of unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @param tup Input tuple
 *
 * @param pred Unary predicate.
 * Must return `bool` for every element of the input tuple.
 * This requirement checked by `static_assert`.
 *
 * @return Returns `true` if `pred` applied to no elements of `tup`
 * return true. Returns `false` otherwise.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_none_of(const Tuple& tup,
                                           Pred&& pred) noexcept -> bool
{
  return not ::supl::tuple_any_of(tup, std::forward<Pred>(pred));
}

namespace impl {

template <typename Tuple, typename T, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_push_back_impl(const Tuple& tup, T&& data,
                     std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::get<Inds>(tup)..., std::forward<T>(data)};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with `data` appended to the end
 * ex. tuple_push_back(std::tuple{2, 4.8}, true) == std::tuple{2, 4.8, true}
 *
 * @tparam Tuple Tuple type
 *
 * @tparam T Type of data to append
 *
 * @param tup Tuple to append to
 *
 * @param data Value to append
 *
 * @return New tuple with `data` appended
 */
/* }}} */
template <typename Tuple, typename T>
[[nodiscard]] constexpr auto tuple_push_back(const Tuple& tup,
                                             T&& data) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::supl::impl::tuple_push_back_impl(tup, std::forward<T>(data),
                                            seq);
}

namespace impl {

template <typename Tuple, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_pop_back_impl(const Tuple& tup,
                    std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::get<Inds>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with the last element removed
 * ex. tuple_pop_back(std::tuple{2, 4.8, true}) == std::tuple{2, 4.8}
 *
 * @tparam Tuple Tuple type
 *
 * @param tup Tuple to remove the last element from
 *
 * @return New tuple with last element removed
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto tuple_pop_back(const Tuple& tup) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return ::supl::impl::tuple_pop_back_impl(tup, seq);
}

namespace impl {

template <typename Tuple, typename T, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_push_front_impl(const Tuple& tup, T&& data,
                      std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::forward<T>(data), std::get<Inds>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with `data` prepended to the beginning
 *
 * @tparam Tuple Tuple type
 *
 * @tparam T Type of data to prepend
 *
 * @param tup Tuple to prepend to
 *
 * @param data Value to prepend
 *
 * @return New tuple with `data` prepended
 */
/* }}} */
template <typename Tuple, typename T>
[[nodiscard]] constexpr auto tuple_push_front(const Tuple& tup,
                                              T&& data) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::supl::impl::tuple_push_front_impl(tup, std::forward<T>(data),
                                             seq);
}

namespace impl {

template <typename Tuple, std::size_t... Inds>
[[nodiscard]] constexpr auto
tuple_pop_front_impl(const Tuple& tup,
                     std::index_sequence<Inds...>) noexcept
{
  return std::tuple {std::get<Inds + 1>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Returns a new tuple with the first element removed
 * ex. tuple_pop_back(std::tuple{2, 4.8, true}) == std::tuple{2, 4.8}
 *
 * @tparam Tuple Tuple type
 *
 * @param tup Tuple to remove the first element from
 *
 * @return New tuple with first element removed
 */
/* }}} */
template <typename Tuple>
[[nodiscard]] constexpr auto tuple_pop_front(const Tuple& tup) noexcept
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple> - 1> {}};
  return ::supl::impl::tuple_pop_front_impl(tup, seq);
}

namespace impl {

template <typename Tuple, typename... T, std::size_t... pre_idxs,
          std::size_t... post_idxs>
[[nodiscard]] constexpr auto
tuple_insert_impl(const Tuple& tup, std::index_sequence<pre_idxs...>,
                  std::index_sequence<post_idxs...>, T&&... data) noexcept
{
  constexpr std::size_t idx {sizeof...(pre_idxs)};

  return std::tuple {std::get<pre_idxs>(tup)..., std::forward<T>(data)...,
                     std::get<post_idxs + idx>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Inserts value(s) into an arbitrary index of a tuple.
 * ex. tuple_insert(std::tuple{3, true}, supl::index_constant<1>, 5.8) 
 * == std::tuple{3, 5.8, true}
 *
 * @tparam Tuple Tuple type
 *
 * @tparam T Type(s) of data to insert
 *
 * @tparam Idx Index to insert at
 *
 * @param tup Tuple to insert into
 *
 * @param deduction_helper Only exists so that `Idx` can be deduced
 *
 * @param data Value(s) to insert
 *
 * @return New tuple with `data` inserted at index `Idx`
 */
/* }}} */
template <typename Tuple, typename... T, std::size_t Idx>
[[nodiscard]] constexpr auto
tuple_insert(const Tuple& tup,
             [[maybe_unused]] supl::index_constant<Idx> deduction_helper,
             T&&... data) noexcept
{
  static_assert(Idx <= std::tuple_size_v<Tuple>, "Index out of bounds");

  auto pre_seq {std::make_index_sequence<Idx> {}};
  auto post_seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - Idx> {}};

  return ::supl::impl::tuple_insert_impl(tup, pre_seq, post_seq,
                                         std::forward<T>(data)...);
}

template <typename Tuple, std::size_t... Idxs>
[[nodiscard]] constexpr auto
tuple_reorder(const Tuple& tup, std::index_sequence<Idxs...>) noexcept
{
  static_assert(((Idxs < std::tuple_size_v<Tuple>)&&...));

  return std::tuple {std::get<Idxs>(tup)...};
}

namespace impl {
template <typename Tuple, std::size_t... Pre_Idxs,
          std::size_t... Post_Idxs, std::size_t Idx>
[[nodiscard]] constexpr auto
tuple_split_impl(const Tuple& tup, std::index_sequence<Pre_Idxs...>,
                 std::index_sequence<Post_Idxs...>,
                 supl::index_constant<Idx>)
{
  return std::pair {std::tuple {std::get<Pre_Idxs>(tup)...},
                    std::tuple {std::get<Post_Idxs + Idx>(tup)...}};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Splits a tuple at an arbitrary index.
 * Element at index `Idx` will be the first element of the second tuple.
 * ex. tuple_split(std::tuple{1, true, 'g'}, index_constant<1>) ==
 * std::pair{std::tuple{1}, std::tuple{true, 'g'}}
 *
 * @tparam Tuple Input tuple type.
 *
 * @tparam Idx Index at which to split.
 *
 * @param tup Tuple to split.
 *
 * @param idx Deduction helper.
 *
 * @return Pair of tuples split from `tup` at index `Idx`.
 * ex. tuple_split(std::tuple{1, true, 'g'}, index_constant<1>) ==
 * std::pair{std::tuple{1}, std::tuple{true, 'g'}}
 */
/* }}} */
template <typename Tuple, std::size_t Idx>
[[nodiscard]] constexpr auto
tuple_split(const Tuple& tup, supl::index_constant<Idx> idx) noexcept
{
  static_assert(Idx < std::tuple_size_v<Tuple>, "Index out of bounds");

  auto pre_seq {std::make_index_sequence<Idx> {}};
  auto post_seq {
      std::make_index_sequence<std::tuple_size_v<Tuple> - Idx> {}};

  return ::supl::impl::tuple_split_impl(tup, pre_seq, post_seq, idx);
}

namespace impl {

template <typename Tuple, std::size_t... Inds, std::size_t begin>
[[nodiscard]] constexpr auto
subtuple_impl(const Tuple& tup,
              [[maybe_unused]] std::index_sequence<Inds...>,
              [[maybe_unused]] supl::index_constant<begin>)
{
  return std::tuple {std::get<Inds + begin>(tup)...};
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Creates a subtuple from a range of indices.
 * Returned tuple will be created from the half-open range [begin, end).
 *
 * @tparam Tuple Tuple type
 *
 * @tparam begin Beginning index
 *
 * @tparam end Ending index
 *
 * @param tup Tuple to extract a subtuple from
 *
 * @param begin_deduct_help Provides deduction for `begin`
 *
 * @param end_deduct_help Provides deduction for `end`
 */
/* }}} */
template <typename Tuple, std::size_t begin, std::size_t end>
[[nodiscard]] constexpr auto
subtuple(const Tuple& tup, supl::index_constant<begin> begin_deduct_help,
         [[maybe_unused]] supl::index_constant<end> end_deduct_help)
{
  static_assert(begin < std::tuple_size_v<Tuple>, "Begin out of bounds");
  static_assert(end <= std::tuple_size_v<Tuple>, "Begin out of bounds");

  auto seq {std::make_index_sequence<end - begin> {}};

  return ::supl::impl::subtuple_impl(tup, seq, begin_deduct_help);
}

/* {{{ doc */
/**
 * @brief Creates a subtuple from a range of indices.
 * Returned tuple will be created from the half-open range [begin, end).
 *
 * @tparam Tuple Tuple type
 *
 * @tparam begin Beginning index
 *
 * @tparam end Ending index
 *
 * @param tup Tuple to extract a subtuple from
 *
 * @param deduction_helper Provides deduction for `begin` and `end`
 */
/* }}} */
template <typename Tuple, std::size_t begin, std::size_t end>
[[nodiscard]] constexpr auto
subtuple(const Tuple& tup, [[maybe_unused]] supl::index_pair<begin, end>
                               deduction_helper) noexcept
{
  return ::supl::subtuple(tup, supl::index_constant<begin> {},
                          supl::index_constant<end> {});
}

/* {{{ doc */
/**
 * @brief Creates a subtuple from a range of indices.
 * Returned tuple will be created from the half-open range [begin, end).
 *
 * @tparam Tuple Tuple type
 *
 * @tparam begin Beginning index
 *
 * @tparam end Ending index
 *
 * @param tup Tuple to extract a subtuple from
 */
/* }}} */
template <typename Tuple, std::size_t begin, std::size_t end>
[[nodiscard]] constexpr auto subtuple(const Tuple& tup) noexcept
{
  return ::supl::subtuple(tup, supl::index_constant<begin> {},
                          supl::index_constant<end> {});
}

namespace impl {

template <typename Tuple, typename Pred, std::size_t... Inds>
constexpr auto tuple_count_if_impl(const Tuple& tup, Pred&& pred,
                                   std::index_sequence<Inds...>) noexcept
    -> std::size_t
{
  return (static_cast<std::size_t>(pred(std::get<Inds>(tup))) + ...);
}

} // namespace impl

/* {{{ doc */
/**
 * @brief Applies a generic predicate to every element of a tuple,
 * returning the number of elements for which that predicate is true.
 *
 * @tparam Tuple Tuple type.
 *
 * @tparam Pred Predicate type
 *
 * @param tup Tuple to apply predicate to elements of.
 *
 * @param pred Generic predicate to apply.
 *
 * @return Number of elements of `tup` such that `pred(tup)` returns `true`.
 */
/* }}} */
template <typename Tuple, typename Pred>
[[nodiscard]] constexpr auto tuple_count_if(const Tuple& tup,
                                            Pred&& pred) noexcept
    -> std::size_t
{
  auto seq {std::make_index_sequence<std::tuple_size_v<Tuple>> {}};
  return ::supl::impl::tuple_count_if_impl(tup, std::forward<Pred>(pred),
                                           seq);
}

} // namespace supl

#endif
