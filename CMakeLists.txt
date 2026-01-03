cmake_minimum_required(VERSION 4.0.1)
project(p2ptransfer)
set(CMAKE_CXX_STANDARD 20) # use c++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require C++ version
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # exporting compile_commands.json for clangd 
set(CMAKE_BUILD_TYPE Debug)

option(TESTING "Build tests" ON)
option(GRAPHICS "Build with Qt" ON) # graphics executable

set(LINK_LIBRARIES ) # libraries to link
set(INCLUDE_DIRECTORIES ) 
set(CMAKE_CXX_FLAGS "-g -frtti -Wall -Wextra -Wpedantic")

if(WIN32)
    set(LINK_LIBRARIES ws2_32 wsock32) 
endif()

find_package(Boost COMPONENTS 
    program_options
)
if(NOT ${Boost_FOUND})
    message(FATAL_ERROR "Boost cannot be found. Try to install it with:\nsudo apt-get install libboost-all-dev")
endif()
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
set(LINK_LIBRARIES ${LINK_LIBRARIES} Boost::program_options)

if(${GRAPHICS})
    find_package(QT NAMES Qt6 REQUIRED Widgets Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)
    if(NOT ${Qt6_FOUND})
        message(FATAL_ERROR "Qt6 cannot be found. Try to install it with:\nsudo apt install qt6-base-dev qt6-declarative-dev qt6-tools-dev qt6-tools-dev-tools")
    endif()
endif()

if(${TESTING})
    find_package(GTest)
    if(NOT ${GTest_FOUND})
        include(FetchContent)
        FetchContent_Declare(
          googletest
          GIT_REPOSITORY https://github.com/google/googletest.git
          GIT_TAG        v1.17.0
          FIND_PACKAGE_ARGS NAMES GTest
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    enable_testing()
endif()

add_subdirectory(
src)
