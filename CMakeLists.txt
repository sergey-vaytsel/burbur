cmake_minimum_required(VERSION 3.19.0)

project(Project)

set(BUILD_SHARED_LIBS Off)
set(CMAKE_CXX_STANDARD 20 CACHE INTERNAL "Default C++ standard for targets")
option(CMAKE_CXX_EXTENSIONS OFF "Turn on compiler specific extensions")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(common NO_POLICY_SCOPE)

include(conan)
get_filename_component(ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}/.." ABSOLUTE CACHE)
conan_config_install(ITEM "${ROOT_DIR}/conan/config")
# Т.к. команда `conan config install` удаляет из настроек ранее добавленные серверы в случае,
# если она встречает в источнике файл remotes.txt, добавим сервер через команду `conan remote add`.
conan_add_remote(NAME bincrafters URL https://bincrafters.jfrog.io/artifactory/api/conan/public-conan)
if(NOT DEFINED CUSTOM_CONAN_OPTIONS)
    set(CUSTOM_CONAN_OPTIONS)
endif()
if(NOT DEFINED CUSTOM_CONAN_PROFILE)
    set(CUSTOM_CONAN_PROFILE)
endif()
conan_cmake_run(
    CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}"
    PROFILE ${CUSTOM_CONAN_PROFILE}
    OPTIONS ${CUSTOM_CONAN_OPTIONS}
    CONANFILE "conan/conanfile.py"
    BASIC_SETUP CMAKE_TARGETS KEEP_RPATHS
    BUILD missing
    NO_OUTPUT_DIRS)

add_subdirectory(src/App)
add_subdirectory(src/Render)
