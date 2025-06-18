add_library(server_view
    gui_presenter.cpp
    mainwindow.cpp
)
target_link_libraries(server_view PUBLIC base)
target_include_directories(server_view PUBLIC ${INCLUDE_PARAMS})
