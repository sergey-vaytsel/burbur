cmake_minimum_required(VERSION 3.19.0)
project(App)

add_executable(App main.cpp Window.h Window.cpp linmath.h)

target_link_libraries(App ${CONAN_LIBS})
