if(${GRAPHICS}) 
    add_library(server_view
        view_gui.cpp
    )
else()
    add_library(server_view
        view_cli.cpp
    )
endif()
target_precompile_headers(server_view REUSE_FROM base)
target_link_libraries(server_view PUBLIC base)
target_include_directories(server_view PUBLIC ${INCLUDE_DIRECTORIES})
