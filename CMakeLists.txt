cmake_minimum_required(VERSION 4.0.1)
project(p2ptransfer)
set(CMAKE_CXX_STANDARD 20) # use c++20
set(CMAKE_CXX_STANDARD_REQUIRED ON) # require C++20
set(CMAKE_EXPORT_COMPILE_COMMANDS 1) # exporting compile_commands.json for neovim
set(CMAKE_BUILD_TYPE Debug)

option(ENABLE_TESTING "build tests or not" OFF) # testing executable
option(GRAPHICS "Build with Qt" ON) # graphics executable

set(LINKAGE_PARAMS ) # libraries to link
set(INCLUDE_PARAMS ../headers) # directories to include

if(WIN32) 
    set(LINKAGE_PARAMS ws2_32 -lmswsock) 
endif()

find_package(Boost REQUIRED)
set(INCLUDE_PARAMS ${INCLUDE_PARAMS} ${Boost_INCLUDE_DIRS})

if(${GRAPHICS})
    find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets Core)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)
    qt_standard_project_setup()
    set(LINKAGE_PARAMS ${LINKAGE_PARAMS} ${Qt6Widgets_LIBRARIES} ${Qt6Core_LIBRARIES})
    set(INCLUDE_PARAMS ${INCLUDE_PARAMS} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Core_INCLUDE_DIRS})
endif()

add_subdirectory(headers)
add_subdirectory(server)
add_subdirectory(client)

if(${ENABLE_TESTING}) 
    add_subdirectory(tests)
endif()
