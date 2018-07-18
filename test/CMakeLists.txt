cmake_minimum_required( VERSION 3.1 )

###############################################################################
# It is assumed that the the Catch2 library header is made available
# in the form of a cmake INTERFACE library with ALIAS Catch::Test
###############################################################################

set( TestProject "test_${Project}" )
project( ${TestProject} )

# find_package(GTest REQUIRED)
# if(GTEST_FOUND)
#     include_directories(
#         ${GTEST_INCLUDE_DIRS}
#     )
# else()
#     message(FATAL_ERROR "Cannot find GoogleTest")
# endif()

###############################################################################
# Prepare test sources

# These are all sources EXCLUDING the tests_main.cpp which contains main()
# function or the appropriate Catch2 define.
# Naming convention is assumed - test_someFeatureUnderTest.cpp
file( GLOB TestSources
      RELATIVE "${PROJECT_SOURCE_DIR}"
      "${PROJECT_SOURCE_DIR}/test_*.cpp"
    )

set( CMAKE_EXPORT_COMPILE_COMMANDS ON )
set( CMAKE_VERBOSE_MAKEFILE OFF )

###############################################################################
# Build executable

# set( TestExecutable "test_${Executable}" )
# main.cpp assumed to contain the main() function or the appropriate
# Catch2 define.
add_executable( ${PROJECT_NAME}
    main.cpp
    )
target_sources( ${PROJECT_NAME}
    PUBLIC ${TestSources}
    )
target_link_libraries( ${PROJECT_NAME}
    project_config
    Lib::InternalLibrary
    GTest::GTest
    GTest::Main
    )

enable_testing()
# gtest_discover_tests(${PROJECT_NAME})
add_test(test_main ${PROJECT_NAME})
