cmake_minimum_required(VERSION 3.5)

set(PICO_BOARD pico)

if (PICO_BOARD STREQUAL "pico")
add_compile_definitions(USING_PICO)
set(PICO_PLATFORM rp2040)
set(PICO_LABEL RPI-RP2)
elseif(PICO_BOARD STREQUAL "pico_w")
add_compile_definitions(USING_PICO_W)
set(PICO_PLATFORM rp2040)
set(PICO_LABEL RPI-RP2)
elseif(PICO_BOARD STREQUAL "pico2")
add_compile_definitions(USING_PICO_2)
set(PICO_PLATFORM rp2350)
set(PICO_LABEL RP2350)
elseif(PICO_BOARD STREQUAL "pico2_w")
add_compile_definitions(USING_PICO_2_W)
set(PICO_PLATFORM rp2350)
set(PICO_LABEL RP2350)
endif()

set(PICO_CXX_ENABLE_RTTI 1)

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

project(Example C CXX ASM)

pico_sdk_init()

option(USING_PRINT "stdout" ON)
option(USING_MULTICORE "multicore" ON)
if (USING_PRINT)
add_compile_definitions(USING_PRINT=1)
set(USE_PRINT_CMAKE 1)
else()
set(USE_PRINT_CMAKE 0)
endif()
if (USING_MULTICORE)
add_compile_definitions(USING_MULTICORE=1)
set(USE_MULTICORE_CMAKE 1)
else()
set(USE_MULTICORE_CMAKE 0)
endif()

add_subdirectory(../ build)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_stdlib
    pico_multicore
    ZipsPicoLib
)

pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})