include(warnings NO_POLICY_SCOPE)

function(add_hlib lib)
    add_library(${lib} INTERFACE)
    target_include_directories(${lib}
        SYSTEM INTERFACE
            ${PROJECT_SOURCE_DIR}/src
    )

    add_library(${lib}_IDE OBJECT ${ARGN})
    set_target_properties(${lib}_IDE PROPERTIES LINKER_LANGUAGE CXX)
endfunction()

function(add_lib lib)
    add_library(${lib} ${ARGN})
    target_link_libraries(${lib}
        PRIVATE
            warnings
    )
    target_include_directories(${lib}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/src
    )
    set_target_properties(${lib} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY $<1:${BUNDLE_OUTPUT_PATH}>
        LIBRARY_OUTPUT_DIRECTORY $<1:${BUNDLE_OUTPUT_PATH}>
    )
endfunction()

function(add_exec exe)
    add_executable(${exe} ${ARGN})
    target_link_libraries(${exe}
        PRIVATE
            warnings
    )
    target_include_directories(${exe}
        PUBLIC
            ${PROJECT_SOURCE_DIR}/src
    )
    set_target_properties(${exe} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY $<1:${BUNDLE_OUTPUT_PATH}>
        LIBRARY_OUTPUT_DIRECTORY $<1:${BUNDLE_OUTPUT_PATH}>
    )
endfunction()