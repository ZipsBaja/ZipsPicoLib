#pragma once

#include "Module.h"

namespace uazips
{   
    template<class T>
    class CollectionModule : public Module
    {
    protected:
        std::vector<T*> m_elements;

    public:
        inline CollectionModule() : Module()
        {
        }
        virtual ~CollectionModule()
        {
        }

    };
}