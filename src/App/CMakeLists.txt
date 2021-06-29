set(MODULE_NAME App)

set(SRC_LIST
    src/main.cpp

    src/Window.h
    src/Window.cpp
)

add_exec(${MODULE_NAME} ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PUBLIC
    Render

    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::fmt
    CONAN_PKG::assimp
)
