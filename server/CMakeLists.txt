set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
    ${CMAKE_CURRENT_SOURCE_DIR}/presenter 
    ${CMAKE_CURRENT_SOURCE_DIR}/model
)
if(${GRAPHICS})
    set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
        ${CMAKE_CURRENT_SOURCE_DIR}/gui
    )
    add_subdirectory(gui)
endif()
add_subdirectory(model)

add_library(server INTERFACE)
target_link_libraries(server INTERFACE 
    server_view 
    server_model 
    ${LINK_LIBRARIES})

qt_add_executable(p2plisten main_graphics.cpp)
target_link_libraries(p2plisten PUBLIC 
    base 
    server
)
target_include_directories(p2plisten PUBLIC ${INCLUDE_DIRECTORIES})
target_compile_options(p2plisten PUBLIC -frtti -Wall -Wextra -Wpedantic)
set(SERVER_INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
