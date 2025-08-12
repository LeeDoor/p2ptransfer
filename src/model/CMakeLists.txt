add_library(server_model
    listener_impl.cpp
    file_processor_impl.cpp
)
target_link_libraries(server_model PUBLIC base)
target_precompile_headers(server_model REUSE_FROM base)
target_include_directories(server_model PUBLIC ${INCLUDE_DIRECTORIES})
