cmake_minimum_required(VERSION 4.0.1)
project(p2ptransfer)
set(CMAKE_CXX_STANDARD 20) # use c++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require C++20
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # exporting compile_commands.json for neovim
set(CMAKE_BUILD_TYPE Debug)

option(TESTING "Build tests" ON)
option(GRAPHICS "Build with Qt" ON) # graphics executable

set(LINK_LIBRARIES ) # libraries to link
set(INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/general) # directories to include

if(WIN32) 
    set(LINK_LIBRARIES ws2_32 -lmswsock) 
endif()

find_package(Boost REQUIRED)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${Boost_INCLUDE_DIRS})

if(${GRAPHICS})
    find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)
    qt_standard_project_setup()
    set(LINK_LIBRARIES ${LINK_LIBRARIES} ${Qt6Widgets_LIBRARIES} ${Qt6Core_LIBRARIES})
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Core_INCLUDE_DIRS})
endif()

if(${TESTING})
    find_package(GTest REQUIRED)
    enable_testing()
endif()

add_subdirectory(general)
add_subdirectory(server)
add_subdirectory(client)
