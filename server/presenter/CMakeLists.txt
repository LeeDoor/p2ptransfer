add_library(server_presenter
    argument_parser.cpp
    presenter.cpp
)
target_link_libraries(server_presenter PUBLIC base)
target_include_directories(server_presenter PUBLIC ${INCLUDE_PARAMS})
