add_library(client_presenter
    presenter.cpp
)
target_precompile_headers(client_presenter REUSE_FROM base)
target_include_directories(client_presenter PUBLIC ${INCLUDE_DIRECTORIES})
target_link_libraries(client_presenter PUBLIC base)
