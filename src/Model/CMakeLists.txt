set(MODULE_NAME Model)

set(SRC_LIST
    Model.h

    src/Model.cpp
)

add_burbur_library(${MODULE_NAME} STATIC ${SRC_LIST})

target_link_libraries(${MODULE_NAME} PUBLIC
    CONAN_PKG::assimp
    CONAN_PKG::fmt
    CONAN_PKG::glm
)
