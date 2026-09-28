cmake_minimum_required(VERSION 3.5)

# Set VSCode Workspace Folder.
set(VSCODE_WORKSPACE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)

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

include($ENV{PICO_SDK_PATH}/pico_sdk_init.cmake)

# Clangd file gen
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (DEFINED VSCODE_WORKSPACE_DIR)
    file(CREATE_LINK ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${VSCODE_WORKSPACE_DIR}/compile_commands.json SYMBOLIC)
else()
    file(CREATE_LINK ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}/compile_commands.json SYMBOLIC)
endif()

project(Example C CXX ASM)

pico_sdk_init()

add_subdirectory(../ build)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    pico_stdlib
    ZipsPicoLib
)

pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 1)
pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 1)
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})