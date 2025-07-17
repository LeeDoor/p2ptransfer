#if(${GRAPHICS}) 
qt_add_library(client_view
        view_gui.ui
        view_gui.cpp
    )
#else()
#add_library(client_view
    #       view_cli.cpp
    #)
#endif()
target_precompile_headers(client_view REUSE_FROM base)
target_link_libraries(client_view PUBLIC base)
target_include_directories(client_view PUBLIC ${INCLUDE_DIRECTORIES})
