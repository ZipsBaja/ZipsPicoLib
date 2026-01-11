add_library(ZipsPicoLib STATIC
    src/util/TimeHandler.cpp
    src/util/VirtualMultiplexer.cpp
)

target_include_directories(ZipsPicoLib PUBLIC
    include
)

add_subdirectory(lib/pico-event-hardware)
add_subdirectory(lib/pico-math)

target_link_libraries(ZipsPicoLib
    pico_stdlib
    pico-event-hardware
    pico-math
)