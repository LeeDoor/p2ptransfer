add_library(server
    argument_parser.cpp 
    manager.cpp
)
target_include_directories(server PUBLIC ${Boost_INCLUDE_DIRS})
add_executable(p2plisten main.cpp)
target_link_libraries(p2plisten PRIVATE server)
