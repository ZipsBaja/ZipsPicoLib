#pragma once

namespace uazips
{

    class Module
    {
    public:
        Module() {}
        virtual ~Module() {}

        virtual void Initialize() = 0;
    };

}