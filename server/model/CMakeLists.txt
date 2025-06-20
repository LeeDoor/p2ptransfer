add_library(server_model
    address_gatherer_impl.cpp
    socket_manager_impl.cpp
    listener_impl.cpp
    file_processor.cpp
    connection_handler.cpp
)
target_link_libraries(server_model PUBLIC base)
target_include_directories(server_model PUBLIC ${INCLUDE_DIRECTORIES})

if(${TESTING})
    add_subdirectory(tests)
endif()
