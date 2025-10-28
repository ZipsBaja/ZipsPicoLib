add_library(ZipsPicoLib STATIC
    src/StaticDefines.cpp
    src/event/Event.cpp
    src/hardware/Button.cpp
    src/hardware/TimeDevice.cpp
    src/hardware/DualActionSensor.cpp
)

target_include_directories(ZipsPicoLib PUBLIC
    include
)

target_link_libraries(ZipsPicoLib
    pico_stdlib
    hardware_irq
    hardware_timer
    pico_multicore
)