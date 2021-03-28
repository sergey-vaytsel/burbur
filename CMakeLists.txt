cmake_minimum_required(VERSION 3.19.0)

project(Project)

include(conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src/App)
