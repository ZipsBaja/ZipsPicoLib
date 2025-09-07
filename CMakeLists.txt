add_library(ZipsPicoLib STATIC
    src/StaticDefines.cpp
)

target_include_directories(ZipsPicoLib INTERFACE
    include
)
