#include <Module.h>
#include <TimeHandler.h>

namespace uazips
{

    std::vector<Module*> Module::InstalledModules = {};

#if USING_MULTICORE
    TimeHandler* TimeHandler::instance = nullptr;
#endif
}
