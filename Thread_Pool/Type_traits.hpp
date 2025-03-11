#pragma once
#include <type_traits>
#include <iterator>

namespace traits {
    //is random access iterator
    template <typename T, typename = void>
    struct is_random_access_iterator : std::false_type {};

    template <typename T>
    struct is_random_access_iterator<T, 
    std::enable_if_t<
    std::is_same_v<
    typename std::iterator_traits<T>::iterator_category, 
    std::random_access_iterator_tag>
    >
    > : std::true_type {};

    //is comparable
    template <typename T, typename = void>
    struct is_comparable : std::false_type {};

    template <typename T>
    struct is_comparable<T, std::void_t<decltype(std::declval<T>() < std::declval<T>())>> : std::true_type {};

    //has_begin_end
    template <typename T, typename = void>
    struct has_begin_end : std::false_type{};

    template <typename T>
    struct has_begin_end<T, std::void_t<
    decltype(std::declval<T>().begin()),
    decltype(std::declval<T>().end())>
    > : std::true_type{};

    //is_readable
    template <typename T, typename = void>
    struct is_dereferenceable : std::false_type {};

    template <typename T>
    struct is_dereferenceable<T, std::void_t<decltype(*std::declval<T>())>> 
    : std::true_type {};

    //is_callable
    template <typename T, typename = void>
    struct is_callable : std::false_type {};

    template <typename T>
    struct is_callable<T, std::void_t<decltype(std::declval<T>()())>> : std::true_type {};
}//namespace traits