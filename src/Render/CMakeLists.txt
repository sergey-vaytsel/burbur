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

add_burbur_library(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PRIVATE
    CONAN_PKG::glad
    CONAN_PKG::glm
    CONAN_PKG::fmt
)

if(GENERATE_TESTS)
    SET(TEST_LIST
        test/CatchMain.cpp
        test/CameraTest.cpp
    )
    add_burbur_test(${MODULE_NAME}
        SOURCE_LIST ${TEST_LIST}
    )
endif()
