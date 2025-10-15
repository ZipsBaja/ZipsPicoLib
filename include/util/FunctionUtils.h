#pragma once

#include <string.h>
#include <functional>

namespace uazips
{

    namespace functionutils
    {
        // Functions that decide whether loops are broken out of.
        // A return of true breaks out the loop, and a return of false stays in the loop.
        namespace breakfunctions
        {
            template<typename T>
            constexpr inline bool ignore(T)
            {
                return false;
            }
            template<typename T>
            constexpr inline bool abort(T)
            {
                return true;
            }
        }

        namespace predicates
        {
            struct CStringCompare
            {
                inline bool operator()(const char* a, const char* b) const
                {
                    if (a == b) return false;
                    if (!a) return true;
                    if (!b) return false;
                    return strcmp(a, b) < 0;
                }
            };
        }
    }

}