set(INCLUDE_PARAMS ${INCLUDE_PARAMS} 
    ${CMAKE_CURRENT_SOURCE_DIR}/presenter 
    ${CMAKE_CURRENT_SOURCE_DIR}/model
)
if(${GRAPHICS})
    set(INCLUDE_PARAMS ${INCLUDE_PARAMS} 
        ${CMAKE_CURRENT_SOURCE_DIR}/gui_view
    )
    add_subdirectory(gui_view)
endif()
add_subdirectory(presenter)
add_subdirectory(model)

add_library(server INTERFACE)
target_link_libraries(server INTERFACE 
    server_view 
    server_model 
    server_presenter 
    ${LINKAGE_PARAMS})

qt_add_executable(p2plisten main_graphics.cpp)
target_link_libraries(p2plisten PUBLIC 
    base 
    server
)
target_include_directories(p2plisten PUBLIC ${INCLUDE_PARAMS})
target_compile_options(p2plisten PUBLIC -frtti -Wall -Wextra -Wpedantic)
set(SERVER_INCLUDE_PARAMS ${INCLUDE_PARAMS} PARENT_SCOPE)
