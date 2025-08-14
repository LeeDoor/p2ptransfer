add_library(server_view INTERFACE)
if(${GRAPHICS}) 
    qt_standard_project_setup()
    add_library(server_view_gui
        general_view_gui.cpp
        listener_view_gui.cpp
    )
    target_link_libraries(server_view_gui PUBLIC pch_graphics ${LINK_LIBRARIES} ${Qt6Widgets_LIBRARIES} ${Qt6Core_LIBRARIES})
    target_include_directories(server_view_gui PUBLIC ${INCLUDE_DIRECTORIES})

    target_link_libraries(server_view INTERFACE server_view_gui)
else()
    add_library(server_view_cli
        general_view_cli.cpp
        listener_view_cli.cpp
        transferer_view_cli.cpp
    )
    target_link_libraries(server_view_cli PUBLIC pch_base ${LINK_LIBRARIES})
    target_include_directories(server_view_cli PUBLIC ${INCLUDE_DIRECTORIES})

    target_link_libraries(server_view INTERFACE server_view_cli)
endif()
