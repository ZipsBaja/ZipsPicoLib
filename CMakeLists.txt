add_library(ZipsPicoLib STATIC
    src/StaticDefines.cpp
)

target_include_directories(ZipsPicoLib PUBLIC
    include
)

target_link_libraries(ZipsPicoLib
    pico_stdlib
    $<IF:${USING_MULTICORE},pico_multicore,"">
)