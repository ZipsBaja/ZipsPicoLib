#include <Module.h>
#include <TimeHandler.h>

namespace uazips
{

    std::vector<Module*> Module::InstalledModules = {};

#if USING_MULTICORE
    TimeHandler* TimeHandler::instance = nullptr;

    void offload_time_updating(TimeHandler* th)
    {
        TimeHandler::instance = th;
        multicore_launch_core1([](){
            while (1)
            {
                TimeHandler::instance->Update();
            }
        });
    }
#endif
}
