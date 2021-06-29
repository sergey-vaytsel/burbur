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

function(add_burbur_library lib)
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

function(add_burbur_executable exe)
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

function(add_burbur_test)
    set(options )
    set(one_value_keywords )
    set(multi_value_keywords SOURCE_LIST)
    cmake_parse_arguments(ADD_BURBUR_TEST "${options}" "${one_value_keywords}" "${multi_value_keywords}" ${ARGN})

    list(GET ADD_BURBUR_TEST_UNPARSED_ARGUMENTS 0 TEST_LIB)

    add_executable(${TEST_LIB}_test ${ADD_BURBUR_TEST_SOURCE_LIST})

    target_link_libraries(${TEST_LIB}_test
        PRIVATE
            ${TEST_LIB}

            CONAN_PKG::catch2
            warnings
    )

    add_test(
        NAME ${TEST_LIB}
        COMMAND ${TEST_LIB}_test
    )
endfunction()
