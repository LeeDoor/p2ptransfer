add_library(client
    client_manager.cpp
    argument_parser.cpp
    network_manager.cpp
    connection_handler.cpp
    request_serializer.cpp
)
target_include_directories(client PUBLIC ${Boost_INCLUDE_DIRS} ../headers) 
target_compile_options(client PUBLIC -frtti -Wall -Wextra -Wpedantic)
add_executable(p2ptransfer main.cpp)
target_link_libraries(p2ptransfer PRIVATE client base)
