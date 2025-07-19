set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/presenter 
    ${CMAKE_CURRENT_SOURCE_DIR}/application
)
#add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(presenter)
if(${TESTING}) 
    #    add_subdirectory(tests)
endif()

add_library(client INTERFACE)
target_link_libraries(client INTERFACE 
    #client_model 
    client_view 
    client_presenter
    ${LINK_LIBRARIES})

#if(${GRAPHICS})
    qt_add_executable(p2ptransfer main_graphics.cpp)
#else()
    #add_executable(p2ptransfer main_cli.cpp)
#endif()
target_link_libraries(p2ptransfer PUBLIC 
    base 
    client 
)
target_include_directories(p2ptransfer PUBLIC ${INCLUDE_DIRECTORIES})
target_precompile_headers(p2ptransfer REUSE_FROM base)
set(SERVER_INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
