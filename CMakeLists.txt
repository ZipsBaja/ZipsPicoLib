add_library(ZipsPicoLib STATIC
    src/StaticDefines.cpp
)

target_include_directories(ZipsPicoLib PUBLIC
    include
)

target_link_libraries(ZipsPicoLib
    pico_stdlib
    $<IF:${USE_MULTICORE_CMAKE},pico_multicore,"">
)