cmake_minimum_required(VERSION 3.13)

project(ZipsPicoLib C CXX ASM)

target_include_directories(ZipsPicoLib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
