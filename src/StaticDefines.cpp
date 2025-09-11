#include <Module.h>
#include <TimeHandler.h>

namespace uazips
{

    std::vector<Module*> Module::InstalledModules = {};

#if USING_MULTICORE
    TimeHandler* TimeHandler::instance = nullptr;

    static void offload_time_updating(TimeHandler* th)
    {
        TimeHandler::instance = th;
        multicore_launch_core1([](){
            TimeHandler::instance->Update();
        });
    }
#endif
}
