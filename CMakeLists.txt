cmake_minimum_required( VERSION 3.1 )

set( Project hello_travis )
project( ${Project} )


###############################################################################
# Prepare external dependencies
###############################################################################
# If using any external libraries create a 'external'
# subdir with an appropriate CMakeLists and add the following
# for each library
# add_subdirectory( external/some_lib )
add_subdirectory( external/google-test )


# Find any external libraries via find_backage
# see cmake --help-module-list and cmake --help-module ModuleName
# for details on a specific module
# If using boost
# find_package( Boost 1.60.0
#   REQUIRED COMPONENTS
#   filesystem system iostreams
#   )
#
# if( Boost_FOUND )
#   include_directories( ${Boost_INCLUDE_DIRS} )
# else()
#   message( FATAL_ERROR "Cannon find Boost" )
# endif()


###############################################################################
# Prepare source files for build
###############################################################################
# Create a Sources variable to all the cpp files necessary
# to build the target
set( MainSources
    ${PROJECT_SOURCE_DIR}/src/main.cpp
   )

# Add all of the remaining source files to be build as a static library
# linked against the main executable
# set( LibrarySources
#      ${PROJECT_SOURCE_DIR}/src/small_test.cpp
#    )
# Or let all of the source files be globed, minus the one given in MainSources
file( GLOB LibrarySources
      "${PROJECT_SOURCE_DIR}/src/*.cpp"
    )
list( REMOVE_ITEM LibrarySources ${MainSources} )


###############################################################################
# Configure build
###############################################################################
# Set required C++ standard
set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
# Set build type
if( NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'Debug' as none was specified.")
  set( CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
endif()

# Export compile_commands.json for use with cppcheck
set( CMAKE_EXPORT_COMPILE_COMMANDS ON )

if(CMAKE_COMPILER_IS_GNUCC)
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

  if(ENABLE_COVERAGE)
    add_compile_options(--coverage -O0)
  endif()
endif()

add_library(project_warnings INTERFACE)
if( CMAKE_CXX_COMPILER_ID MATCHES "MSVC" )
    target_compile_options( project_warnings INTERFACE /W4 /WX )
else()
    target_compile_options( project_warnings INTERFACE
        -Wall
        -Wextra # reasonable and standard
        -Wshadow # warn the user if a variable declaration shadows one from a
                # parent context
        -Wnon-virtual-dtor # warn the user if a class with virtual functions has a
                        # non-virtual destructor. This helps catch hard to
                        # track down memory errors
        -Wold-style-cast # warn for c-style casts
        -Wcast-align # warn for potential performance problem casts
        -Wunused # warn on anything being unused
        -Woverloaded-virtual # warn if you overload (not override) a virtual
                            # function
        -Wpedantic # warn if non-standard C++ is used
        -Wconversion # warn on type conversions that may lose data
        -Wsign-conversion # warn on sign conversions
        -Wmisleading-indentation # warn if identation implies blocks where blocks
                                # do not exist
        -Wduplicated-cond # warn if if / else chain has duplicated conditions
        -Wduplicated-branches # warn if if / else branches have duplicated code
        -Wlogical-op # warn about logical operations being used where bitwise were
                    # probably wanted
        -Wnull-dereference # warn if a null dereference is detected
        -Wuseless-cast # warn if you perform a cast to the same type
        -Wdouble-promotion # warn if float is implicit promoted to double
        -Wformat=2 # warn on security issues around functions that format output
                # (ie printf) 
    )
endif()
find_program(Cppcheck NAMES cppcheck)
if (Cppcheck)
    list(
        APPEND Cppcheck 
            "-I${PROJECT_SOURCE_DIR}/include"
            "--enable=all"
            "--inconclusive"
            "--force"
            "--inline-suppr"
    )
    message(${Cppcheck})
endif()

find_program(Clangtidy NAMES clang-tidy)
if (Clangtidy)
    list(
        APPEND Clangtidy 
            "-checks='*'"
    )
    message(${Clangtidy})
endif()
# set(CMAKE_CXX_CLANG_TIDY ${Clangtidy})
# set(CMAKE_CXX_CPPCHECK ${Cppcheck})
###############################################################################
# Build target
###############################################################################
# Internal library first
set( InternalLibrary "${Project}_internallib" ) # cmake will add a "lib" prefix to the name


add_library( ${InternalLibrary}
STATIC
${LibrarySources}
)
set_target_properties( ${InternalLibrary}
    PROPERTIES
    CXX_CPPCHECK "${Cppcheck}"
    CXX_CLANG_TIDY "${Clangtidy}"
    )
add_library( Lib::InternalLibrary
    ALIAS ${InternalLibrary}
    )
target_include_directories( ${InternalLibrary}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    )
target_link_libraries( ${InternalLibrary}
    project_warnings --coverage
    # ${Boost_LIBRARIES}
    )


# Then the executable
add_executable( ${PROJECT_NAME}
    ${MainSources}
    )
    set_target_properties( ${PROJECT_NAME}
    PROPERTIES
    CXX_CPPCHECK "${Cppcheck}"
    CXX_CLANG_TIDY "${Clangtidy}"
    )
    target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)
    # If using any external libraries include them here after the Internal Library.
    # They need to be properly found first. See find_package section
    target_link_libraries( ${PROJECT_NAME}
    project_warnings --coverage
    Lib::InternalLibrary
    # ${Boost_LIBRARIES}
    )



###############################################################################
# Unit Tests
###############################################################################
add_subdirectory( test )
