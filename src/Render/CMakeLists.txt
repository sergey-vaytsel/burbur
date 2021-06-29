set(MODULE_NAME Render)

set(SRC_LIST
    Camera.h
    Render.h
    Scene.h
    ShaderProgramBuilder.h

    src/Camera.cpp
    src/Render.cpp
    src/Scene.cpp
    src/ShaderProgramBuilder.cpp
)

add_lib(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glm
    CONAN_PKG::fmt
)
