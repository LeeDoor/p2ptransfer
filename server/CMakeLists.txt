add_library(server
    argument_parser.cpp 
    connection_handler.cpp
    server_manager.cpp
    network_manager.cpp
)
target_include_directories(server PUBLIC ${Boost_INCLUDE_DIRS} ../headers)
target_compile_options(server PRIVATE -frtti -Wall -Wextra -Wpedantic)
add_executable(p2plisten main.cpp)
target_link_libraries(p2plisten PUBLIC server base)
