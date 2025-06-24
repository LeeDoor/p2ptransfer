add_library(server_view
    view_factory_impl.cpp
    view_gui.cpp
)
target_link_libraries(server_view PUBLIC base)
target_include_directories(server_view PUBLIC ${INCLUDE_DIRECTORIES})
