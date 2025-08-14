add_library(server_presenter
    general_presenter.cpp
    listener_presenter.cpp
    transferer_presenter.cpp
    signal_handler.cpp
)
target_link_libraries(server_presenter PUBLIC pch_base)
target_include_directories(server_presenter PUBLIC ${INCLUDE_DIRECTORIES})
