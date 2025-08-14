add_library(server_model
    transferer_impl.cpp
    listener_impl.cpp
    file_reader_impl.cpp
    address_gatherer_impl.cpp
    request_serializer.cpp
    request_deserializer.cpp
    string_view_methods.cpp
    thread_wrapper_impl.cpp
)
target_link_libraries(server_model PUBLIC pch_base)
target_include_directories(server_model PUBLIC ${INCLUDE_DIRECTORIES})
