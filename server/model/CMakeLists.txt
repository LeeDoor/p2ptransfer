add_library(server_model
    address_gatherer.cpp
    connection_handler.cpp
    network_manager.cpp
)
target_link_libraries(server_model PUBLIC base)
target_include_directories(server_model PUBLIC ${INCLUDE_PARAMS})
