set(MODULE_NAME App)

set(SRC_LIST
    src/main.cpp

    src/Window.h
    src/Window.cpp
    src/linmath.h
)

project(${MODULE_NAME})

add_exec(${MODULE_NAME} ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PUBLIC
    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::opengl

    Render
)
