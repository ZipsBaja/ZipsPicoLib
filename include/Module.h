#pragma once

#include <vector>

namespace uazips
{

    class Module
    {
    public:
        static std::vector<const Module*> InstalledModules;

    public:
        inline Module()
        {
            InstalledModules.push_back(this);
        }
        virtual ~Module() {}

        template<class Mod, typename InType>
        static bool IsModuleAType(const InType&& module)
        {
            if constexpr !(std::is_pointer_v<std::decay_t<InType>>)
                IsModuleAType<Mod>(module);
            else
                return dynamic_cast<Mod*>(module) != nullptr;
        }

        template<class Mod>
        static std::vector<Mod> GetModulesByType()
        {
            std::vector<Mod*> modules;
            for (auto&& mod : InstalledModules)
                if (IsModuleAType<Mod>(mod))
                    modules.push_back(mod);
            return modules;
        }

        virtual void Initialize() = 0;
    };

    std::vector<const Module*> Module::InstalledModules = {};

}