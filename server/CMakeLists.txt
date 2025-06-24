set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
    ${CMAKE_CURRENT_SOURCE_DIR}/model
    ${CMAKE_CURRENT_SOURCE_DIR}/view
    ${CMAKE_CURRENT_SOURCE_DIR}/presenter 
)
add_subdirectory(model)
add_subdirectory(view)
add_subdirectory(presenter)

add_library(server INTERFACE)
target_link_libraries(server INTERFACE 
    server_model 
    server_view 
    server_presenter
    ${LINK_LIBRARIES})

if(${GRAPHICS})
    qt_add_executable(p2plisten main_graphics.cpp)
else()
    message( FATAL_ERROR "graphicsless server is not implemented")
endif()
target_link_libraries(p2plisten PUBLIC 
    base 
    server
)
target_include_directories(p2plisten PUBLIC ${INCLUDE_DIRECTORIES})
target_compile_options(p2plisten PUBLIC -frtti -Wall -Wextra -Wpedantic)
set(SERVER_INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
