#pragma once

#if __cplusplus >= 202002L

#include <concepts>

template<class Type, class Base>
concept Extends = std::is_base_of_v<Base, Type>;

#endif