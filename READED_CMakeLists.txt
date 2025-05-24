cmake_minimum_required(VERSION 4.0.1)
project(p2ptransfer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
option(ENABLE_TESTING "build tests or not" ON)
find_package(Boost REQUIRED)

add_subdirectory(headers)

add_subdirectory(server)
add_subdirectory(client)

if(${ENABLE_TESTING}) 
    add_subdirectory(tests)
endif()
