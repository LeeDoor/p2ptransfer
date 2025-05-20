add_library(server
    argument_parser.cpp 
    manager.cpp
    network_manager.cpp
)
target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
add_executable(p2plisten main.cpp)
target_link_libraries(p2plisten PUBLIC server)
target_compile_options(p2plisten PRIVATE -Wall -Wextra -Wpedantic -Werror)
