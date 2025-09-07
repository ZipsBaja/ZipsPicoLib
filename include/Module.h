#pragma once

#include <ZipsLib.h>

#include <vector>
#include <typeinfo>

namespace uazips
{

    class Module
    {
    public:
        static std::vector<Module*> InstalledModules;

    public:
        inline Module()
        {
            InstalledModules.push_back(this);
        }
        virtual ~Module() {}

        template<class Mod, typename InType>
        static inline bool IsModuleAType(InType&& module)
        {
            if constexpr (!(std::is_pointer_v<std::decay_t<InType>>))
                IsModuleAType<Mod>(module);
            else
                return dynamic_cast<Mod*>(module) != nullptr;
        }

        template<class Mod>
        static inline std::vector<Mod> GetModulesByType()
        {
            std::vector<Mod*> modules;
            for (auto&& mod : InstalledModules)
                if (IsModuleAType<Mod>(mod))
                    modules.push_back(mod);
            return modules;
        }

        virtual bool Initialize() = 0;

        static inline void InitAll()
        {
            for (auto&& mod : InstalledModules)
                if (mod->Initialize())
                    LOG("Module of type %s initialized.\n", typeid(*mod).name());
                else
                    LOG("Module of type %s failed to initialize.\n", typeid(*mod).name());
        }
    };
}