cmake_minimum_required(VERSION 4.0.1)
project(p2ptransfer)
set(CMAKE_CXX_STANDARD 20) # use c++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require C++ version
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # exporting compile_commands.json for neovim
set(CMAKE_BUILD_TYPE Debug)

option(TESTING "Build tests" ON)
option(GRAPHICS "Build with Qt" ON) # graphics executable

set(LINK_LIBRARIES ) # libraries to link
set(CMAKE_CXX_FLAGS "-frtti -Wall -Wextra -Wpedantic -Werror -Wno-unused-includes")
if(WIN32) 
    set(LINK_LIBRARIES ws2_32 -lmswsock) 
endif()

find_package(Boost REQUIRED COMPONENTS 
    stacktrace_basic
    stacktrace_backtrace
    stacktrace_addr2line
    stacktrace_noop
)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})
set(LINK_LIBRARIES 
    ${Boost_STACKTRACE_BASIC_LIBRARY}
    ${Boost_STACKTRACE_BACKTRACE_LIBRARY}
    ${Boost_STACKTRACE_ADDR2LINE_LIBRARY}
    ${Boost_STACKTRACE_NOOP_LIBRARY}
pthread dl backtrace)

if(${GRAPHICS})
    find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)
endif()

if(${TESTING})
    find_package(GTest REQUIRED)
    enable_testing()
endif()

add_subdirectory(src)
