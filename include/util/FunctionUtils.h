#pragma once

namespace uazips
{

    namespace functionutils
    {
        // Functions that decide whether loops are broken out of.
        // A return of true breaks out the loop, and a return of false stays in the loop.
        namespace breakfunctions
        {
            constexpr inline bool ignore()
            {
                return false;
            }

            constexpr inline bool abort()
            {
                return true;
            }
        }
    }

}