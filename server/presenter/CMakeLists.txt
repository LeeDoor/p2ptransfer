add_library(server_presenter
    presenter_impl.cpp
)
target_precompile_headers(server_presenter REUSE_FROM base)
target_include_directories(server_presenter PUBLIC ${INCLUDE_DIRECTORIES})
target_link_libraries(server_presenter PUBLIC base)
