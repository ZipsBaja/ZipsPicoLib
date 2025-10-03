add_library(ZipsPicoLib STATIC
    src/StaticDefines.cpp
    src/event/Event.cpp
    src/hardware/IODevice.cpp
    src/hardware/Button.cpp
    src/hardware/TimeDevice.cpp
)

target_include_directories(ZipsPicoLib PUBLIC
    include
)

target_link_libraries(ZipsPicoLib
    pico_stdlib
    hardware_irq
    hardware_timer
    $<IF:${USE_MULTICORE_CMAKE},pico_multicore,>
)