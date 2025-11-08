set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
    ${CMAKE_CURRENT_SOURCE_DIR}/headers
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/presenter 
    ${CMAKE_CURRENT_SOURCE_DIR}/application
)
include(pch.cmake)
add_subdirectory(model)
add_subdirectory(presenter)
add_subdirectory(view)
if(${TESTING}) 
    add_subdirectory(tests)
endif()

add_library(server INTERFACE)
target_link_libraries(server INTERFACE
    server_model 
    server_view
    server_presenter
    ${LINK_LIBRARIES}
)
target_include_directories(server INTERFACE ${INCLUDE_DIRECTORIES})
if(${GRAPHICS})
    qt_add_executable(p2ptransfer_gui main_graphics.cpp)
    target_link_libraries(p2ptransfer_gui PRIVATE server ${LINK_LIBRARIES})
    if(WIN32)
        set_target_properties(p2ptransfer_gui PROPERTIES
            LINK_FLAGS "-Wl,-subsystem,windows"
        )
    endif()
else()
    add_executable(p2ptransfer_cli main_cli.cpp)
    target_link_libraries(p2ptransfer_cli PRIVATE server ${LINK_LIBRARIES})
endif()
set(SERVER_INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
