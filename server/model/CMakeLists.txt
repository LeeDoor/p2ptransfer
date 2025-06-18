add_library(server_model
    address_gatherer.cpp
    file_processor.cpp
    network_manager.cpp
)
target_link_libraries(server_model PUBLIC base)
target_include_directories(server_model PUBLIC ${INCLUDE_PARAMS})
