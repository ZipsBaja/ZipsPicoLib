# Akron Zips Baja Raspberry Pi Pico Library

This (almost) header-only library is used in the projects using the Raspberry Pi Pico family microcontrollers.
To use this library, either clone or add this repository as a submodule. It is recommended to add as a submodule.
`git submodule add https://github.com/ZipsBaja/ZipsPicoLib <library directory>`

In your CMakeLists.txt file, add the following lines:
```
add_subdirectory(<library directory>)
target_link_libraries(${CMAKE_PROJECT_NAME} ZipsPicoLib)
```
Of course, replace the directory with where you cloned the repo.

---
Here's an example CMakeLists.txt entry:
```
cmake_minimum_required(VERSION 3.13)
project(MyProject C CXX ASM)
add_subdirectory(lib/zipslib)
target_link_libraries(${CMAKE_PROJECT_NAME} ZipsPicoLib)
```
