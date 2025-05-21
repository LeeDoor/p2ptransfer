add_library(client
    client_manager.cpp
    argument_parser.cpp
)
target_include_directories(client PUBLIC ${Boost_INCLUDE_DIRS} ../headers) 
add_executable(p2ptransfer main.cpp)
target_link_libraries(p2ptransfer PRIVATE client)
