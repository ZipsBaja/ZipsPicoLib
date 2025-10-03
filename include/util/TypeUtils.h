#pragma once

#if __cplusplus >= 202002L
#include <concepts>
template<class Type, class Base>
concept Extends = std::is_base_of_v<Base, Type>;
#else
#include <type_traits>
template<class Type, class Base>
using Extends = std::is_base_of_v<Base, Type>;
#endif