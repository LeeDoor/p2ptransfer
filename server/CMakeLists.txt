add_library(server
    argument_parser.cpp 
    connection_handler.cpp
    server_manager.cpp
    network_manager.cpp
)
target_link_libraries(server PRIVATE base)
target_include_directories(server PUBLIC ${INCLUDE_PARAMS})
target_compile_options(server PRIVATE -frtti -Wall -Wextra -Wpedantic)
if(${GRAPHICS})
    qt_add_executable(p2plisten main_graphics.cpp)
else()
    add_executable(p2plisten main.cpp)
endif()
target_link_libraries(p2plisten PUBLIC server ${LINKAGE_PARAMS})
