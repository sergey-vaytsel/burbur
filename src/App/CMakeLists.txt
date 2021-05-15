set(MODULE_NAME App)

set(SRC_LIST
    src/main.cpp

    src/Object.h
    src/Object.cpp
    src/Render.h
    src/Render.cpp
    src/Window.h
    src/Window.cpp
)

project(${MODULE_NAME})

add_exec(${MODULE_NAME} ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PUBLIC
    CONAN_PKG::glad
    CONAN_PKG::glfw
    CONAN_PKG::glm
    CONAN_PKG::fmt
    CONAN_PKG::assimp
)
