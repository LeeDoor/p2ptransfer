add_library(server
    argument_parser.cpp 
    connection_handler.cpp
    server_manager.cpp
    network_manager.cpp
)
target_include_directories(server PUBLIC ${Boost_INCLUDE_DIRS} ../headers)
add_executable(p2plisten main.cpp)
target_link_libraries(p2plisten PUBLIC server)
target_compile_options(p2plisten PRIVATE -Wall -Wextra -Wpedantic -Werror)
