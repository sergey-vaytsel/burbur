cmake_minimum_required(VERSION 3.19.0)

project(Project)

set(BUILD_SHARED_LIBS Off)
set(CMAKE_CXX_STANDARD 17 CACHE INTERNAL "Default C++ standard for targets")
option(CMAKE_CXX_EXTENSIONS OFF "Turn on compiler specific extensions")

include(conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src/App)
