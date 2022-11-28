#ifndef SUPPLEMENTARIES_ALGORITHM_HPP
#define SUPPLEMENTARIES_ALGORITHM_HPP

#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <type_traits>
#include <utility>

namespace supl {

/* {{{ doc */
/**
 * @brief Base case of recursive overload.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @param cont Container whose size to query.
 *
 * @return Size of container `cont`.
 */
/* }}} */
template <typename Container>
constexpr auto min_size(const Container& cont) noexcept -> std::size_t
{
  return cont.size();
}

/* {{{ doc */
/**
 * @brief Returns size of smallest container provided.
 * Calls size method internally.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @tparam Containers Sequence of container types which all provide
 * a `.size()` member function. Types may be mixed, so long as all
 * `.size()` member functions return comparable types which can be
 * passed to `std::min`.
 *
 * @param cont First container.
 *
 * @param conts Pack of remaining containers.
 *
 * @return Size of smallest container.
 */
/* }}} */
template <typename Container, typename... Containers>
constexpr auto min_size(const Container& cont,
                        const Containers&... conts) noexcept -> std::size_t
{
  return std::min(::supl::min_size(cont), ::supl::min_size(conts...));
}

/* {{{ doc */
/**
 * @brief Base case of recursive overload.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @param cont Container whose size to query.
 *
 * @return Size of container `cont`.
 */
/* }}} */
template <typename Container>
constexpr auto max_size(const Container& cont) noexcept -> std::size_t
{
  return cont.size();
}

/* {{{ doc */
/**
 * @brief Returns size of largest container provided.
 * Calls size method internally.
 *
 * @tparam Container Container which provides a `.size()`
 * member function.
 *
 * @tparam Containers Sequence of container types which all provide
 * a `.size()` member function. Types may be mixed, so long as all
 * `.size()` member functions return comparable types which can be
 * passed to `std::max`.
 *
 * @param cont First container.
 *
 * @param conts Pack of remaining containers.
 *
 * @return Size of smallest container.
 */
/* }}} */
template <typename Container, typename... Containers>
constexpr auto max_size(const Container& cont,
                        const Containers&... conts) noexcept -> std::size_t
{
  return std::max(::supl::max_size(cont), ::supl::max_size(conts...));
}

template <typename Itr>
constexpr auto
contains(const Itr begin, const Itr end,
         // oh boy
         std::add_lvalue_reference_t<
             // ref to
             std::add_const_t<
                 //const
                 typename std::iterator_traits<Itr>::value_type>>
             // basically: decltype(value) == const Itr::value_type &
             value) noexcept(noexcept(std::find(begin, end, value)))
    -> bool
{
  return std::find(begin, end, value) != end;
}

/* {{{ doc */
/**
 * @brief Conditionally transforms each element of a range.
 *
 * @tparam Itr Input iterator type. A forward iterator is sufficient.
 *
 * @tparam OutItr Output iterator type.
 * `OutItr = value; OutItr = another;` must insert `value` then `another`
 * into the output range to produce expected behavior.
 * Specializations of `std::back_insert_iterator`, `std::insert_iterator`,
 * and `std::front_insert_iterator` satisfy the preconditions, so long as
 * the underlying container contains an appropriate type.
 *
 * @tparam Predicate Type of unary predicate callable. Must accept a
 * single parameter of type `Itr::value_type`, and return a `bool`.
 *
 * @tparam TransformFunc Type of unary transforming callable. Must accept a
 * single parameter of type `Itr::value_type`, and return a value
 * implicitly convertible to `OutItr::container_type::value_type`.
 *
 * @param begin Beginning of input range.
 *
 * @param end End of input range.
 *
 * @param output_itr Output iterator to range used as destination of
 * transformed values.
 *
 * @param pred Unary predicate used to determine if an input value will be
 * transformed.
 *
 * @param func Unary function used to transform data.
 */
/* }}} */
template <typename Itr, typename OutItr, typename Predicate,
          typename TransformFunc>
constexpr void
transform_if(Itr begin, const Itr end, OutItr output_itr, Predicate&& pred,
             TransformFunc&& func) noexcept(noexcept(func(*begin)))
{
  for ( ; begin != end; ++begin ) {
    if ( pred(*begin) ) {
      output_itr = func(*begin);
    }
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each adjacent pair of elements.
 *
 * Example: Range is: {1, 2, 3}
 *
 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
 *
 * @tparam Itr Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin Iterator to the beginning of the range.
 *
 * @param end Iterator to the end of the range.
 *
 * @param func Binary function to apply to each adjacent pair.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
constexpr void for_each_adjacent(
    const Itr begin, const Itr end,
    BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  Itr leader {std::next(begin)};
  Itr follower {begin};

  for ( ; leader != end; ++leader, ++follower ) {
    func(*leader, *follower);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each adjacent pair of elements.
 * Iteration ceases when the range runs out of adjacent pairs,
 * or when `n` iterations have occurred; whichever comes first.
 *
 * Example: Range is: {1, 2, 3}
 *
 * Calls to `func` will be: `func(1, 2)`, `func(2, 3)`
 *
 * @tparam Itr Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin Iterator to the beginning of the range.
 *
 * @param end Iterator to the end of the range.
 *
 * @param n Maximum number of calls to make.
 *
 * @param func Binary function to apply to each adjacent pair.
 */
/* }}} */
template <typename Itr, typename BinaryFunc>
constexpr void for_each_adjacent_n(
    const Itr begin, const Itr end, const std::size_t n,
    BinaryFunc&& func) noexcept(noexcept(func(*begin, *begin)))
{
  std::size_t count {0};
  Itr leader {std::next(begin)};
  Itr follower {begin};

  for ( ; count != n && leader != end; ++count, ++leader, ++follower ) {
    func(*leader, *follower);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each corresponding pair of elements.
 * Iteration ceases when either range runs out of members.
 *
 * Example: Range 1 is: {1, 2, 3}, Range 2 is: {4, 5, 6, 7}
 *
 * Calls to `func` will be:
 *
 * * `func(1, 4)`
 * * `func(2, 5)`
 * * `func(3, 6)`
 *
 * @tparam Itr1 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam Itr2 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin1 Iterator to the beginning of range 1
 *
 * @param end1 Iterator to the end of range 1
 *
 * @param begin2 Iterator to the beginning of range 2
 *
 * @param end2 Iterator to the end of range 2
 *
 * @param func Binary function to apply to each corresponding pair
 */
/* }}} */
template <typename Itr1, typename Itr2, typename BinaryFunc>
constexpr void
for_each_both(Itr1 begin1, const Itr1 end1, Itr2 begin2, const Itr2 end2,
              BinaryFunc&& func) noexcept(noexcept(func(*begin1, *begin2)))
{
  for ( ; (begin1 != end1 && begin2 != end2); ++begin1, ++begin2 ) {
    func(*begin1, *begin2);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to each corresponding pair of elements.
 * Iteration ceases when either range runs out of members,
 * or when `n` iterations have occurred; whichever comes first.
 *
 * Example: Range 1 is: {1, 2, 3}, Range 2 is: {4, 5, 6, 7}
 *
 * Calls to `func` will be:
 *
 * * `func(1, 4)`
 * * `func(2, 5)`
 * * `func(3, 6)`
 *
 * @tparam Itr1 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam Itr2 Iterator type satisfying only a minimal interface.
 * Simply being incrementable and equality-comparable is sufficient.
 *
 * @tparam BinaryFunc Function which takes two arguments, and
 * for both must accept the type which the iterators point to.
 *
 * @param begin1 Iterator to the beginning of range 1
 *
 * @param end1 Iterator to the end of range 1
 *
 * @param begin2 Iterator to the beginning of range 2
 *
 * @param end2 Iterator to the end of range 2
 *
 * @param n Maximum number of iterations.
 *
 * @param func Binary function to apply to each corresponding pair
 */
/* }}} */
template <typename Itr1, typename Itr2, typename BinaryFunc>
constexpr void for_each_both_n(
    Itr1 begin1, const Itr1 end1, Itr2 begin2, const Itr2 end2,
    const std::size_t n,
    BinaryFunc&& func) noexcept(noexcept(func(*begin1, *begin2)))
{
  for ( std::size_t count {0};
        (count != n && begin1 != end1 && begin2 != end2);
        ++count, ++begin1, ++begin2 ) {
    func(*begin1, *begin2);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to members of
 * all containers in parameter order.
 * Iteration ceases when after `n` iterations.
 * If `n` is greater than the size of the smallest container,
 * behavior is undefined.
 *
 * Example: Range1: {1, 2, 3}, Range2: {'a', 'b', 'c'}
 *
 * `func` must have accept `int, char` as parameters.
 *
 * @tparam VarFunc A function which acccepts the types of all the
 * contaners in parameter order.
 *
 * @tparam Begins Sequence of iterators satisfying only a minimal
 * inteface. Simply being incrementable is sufficient.
 *
 * @param func A function which accepts the types of all the containers
 * in parameter order.
 *
 * @param n Maximum number of iterations. Must not be greater
 * than the size of the smallest container.
 *
 * @param begins Iterators to containers to be iterated over.
 * Smallest distance from any to its corresponding end must not be less
 * than `n`, or behavior will be undefined.
 */
/* }}} */
template <typename VarFunc, typename... Begins>
constexpr void
for_each_all_n(VarFunc&& func, const std::size_t n,
               Begins... begins) noexcept(noexcept(func(*begins...)))
{
  for ( std::size_t i {0}; i != n; ++i ) {
    func(*begins...);
    (++begins, ...);
  }
}

/* {{{ doc */
/**
 * @brief Applies `func` to members of all container in
 * parameter order. Iteration ceases upon reaching the end of
 * any container.
 *
 * @tparam VarFunc A function which acccepts the types of all the
 * contaners in parameter order.
 *
 * @tparam Containers Sequence of containers which provide
 * iterators capable of incrementing and dereferencing.
 *
 * @param func A function which accepts the types of all the containers
 * in parameter order.
 *
 * @param containers Parameter pack of containers to be
 * iterated over.
 */
/* }}} */
template <typename VarFunc, typename... Containers>
constexpr void
for_each_all(VarFunc&& func, Containers&... containers) noexcept(
    noexcept(::supl::for_each_all_n(std::forward<VarFunc>(func),
                                    ::supl::min_size(containers...),
                                    std::begin(containers)...)))
{
  ::supl::for_each_all_n(std::forward<VarFunc>(func),
                         ::supl::min_size(containers...),
                         std::begin(containers)...);
}

/* {{{ doc */
/**
 * @brief Applies `func` to members of all container in
 * parameter order. Iteration ceases upon reaching the end of
 * any container. Ensures the containers are not modified.
 *
 * @tparam VarFunc A function which acccepts the types of all the
 * contaners in parameter order.
 *
 * @tparam Containers Sequence of containers which provide
 * iterators capable of incrementing and dereferencing.
 *
 * @param func A function which accepts the types of all the containers
 * in parameter order.
 *
 * @param containers Parameter pack of const containers to be
 * iterated over.
 */
/* }}} */
template <typename VarFunc, typename... Containers>
constexpr void
for_each_all_c(VarFunc&& func, const Containers&... containers) noexcept(
    noexcept(for_each_all_n(std::forward<VarFunc>(func),
                            ::supl::min_size(containers...),
                            std::cbegin(containers)...)))
{
  ::supl::for_each_all_n(std::forward<VarFunc>(func),
                         ::supl::min_size(containers...),
                         std::cbegin(containers)...);
}

inline namespace bkprt {

/* {{{ doc */
/**
 * @brief Re-implementation of std::generate usable in a constexpr context.
 * Redundant if using >=C++20.
 *
 * @tparam Itr Iterator type
 *
 * @tparam Gen Parameterless function which returns `Itr::value_type`
 *
 * @param begin Beginning of range to be generated over.
 *
 * @param end End of range to be generated over.
 *
 * @param gen Parameterless function which returns `Itr::value_type`
 */
/* }}} */
template <typename Itr, typename Gen>
constexpr void
generate(Itr begin, const Itr end,
         Gen&& gen) noexcept(noexcept(gen()) && noexcept(*begin))
{
  while ( begin != end ) {
    *begin = gen();
    ++begin;
  }
}

} // namespace bkprt

} // namespace supl

#endif
