set(MODULE_NAME App)

set(SRC_LIST
    src/main.cpp

    src/Widget.h
    src/Widget.cpp
    src/Utils.h
    src/Utils.cpp

    ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_glfw.cpp
    ${CONAN_SRC_DIRS_IMGUI}/imgui_impl_opengl3.cpp
)

include_directories(${CONAN_SRC_DIRS_IMGUI})

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
