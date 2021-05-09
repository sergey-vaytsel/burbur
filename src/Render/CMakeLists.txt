set(MODULE_NAME Render)

set(SRC_LIST
    Render.h
    src/Render.cpp
)

add_lib(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME}
    PUBLIC
    CONAN_PKG::glm
    CONAN_PKG::glad
)
