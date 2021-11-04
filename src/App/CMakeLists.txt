set(MODULE_NAME App)

set(SRC_LIST
    src/main.cpp

    src/Window.h
    src/Window.cpp
    src/Utils.h
    src/Utils.cpp
)

add_burbur_executable(${MODULE_NAME} ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PUBLIC
    Render
    Model

    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::fmt
    CONAN_PKG::imgui
)
