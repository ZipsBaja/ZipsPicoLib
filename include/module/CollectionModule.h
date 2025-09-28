#pragma once

#include "Module.h"

namespace uazips
{   
    template<class T>
    class CollectionModule : public Module
    {
    protected:
        std::vector<T*> m_elements;

        inline size_t FindElement(const T* element_to_find)
        {
            for (size_t i = 0; i < m_elements.size(); i++)
                if (m_elements[i] == element_to_find)
                    return i;
        }

    public:
        inline CollectionModule() : Module()
        {
        }
        inline virtual ~CollectionModule()
        {
        }

    };
}