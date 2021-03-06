set(MODULE_NAME Render)

set(SRC_LIST
    Camera.h
    Render.h
    Scene.h
    Shader.h
    Shader.inl
    ShaderProgramBuilder.h

    src/Camera.cpp
    src/Render.cpp
    src/Scene.cpp
    src/Shader.cpp
    src/ShaderProgramBuilder.cpp
)

add_burbur_library(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glm
    CONAN_PKG::fmt
)

if(GENERATE_TESTS)
    SET(TEST_LIST
        test/CameraTest.cpp
        test/CatchMain.cpp
        test/TestUtils.h
    )
    add_burbur_test(${MODULE_NAME}
        SOURCE_LIST ${TEST_LIST}
        DEPS_LIST CONAN_PKG::glm
    )
endif()
