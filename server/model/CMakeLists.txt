add_library(server_model
    model_impl_factory.cpp
    address_gatherer_impl.cpp
    listener_impl.cpp
    file_processor_impl.cpp
    connection_establisher_impl.cpp
)
target_link_libraries(server_model PUBLIC base)
target_include_directories(server_model PUBLIC ${INCLUDE_DIRECTORIES})
