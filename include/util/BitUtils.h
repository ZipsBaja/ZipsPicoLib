#pragma once

namespace uazips
{

    template<typename Single, typename Many>
    union ByteSplitter
    {
        Single single;
        Many many[sizeof(Single)/sizeof(Many)];
    };

    template<typename Single, typename Many>
    using ByteCombiner = ByteSplitter<Single, Many>;

}