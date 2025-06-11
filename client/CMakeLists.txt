set (CLIENT_SOURCE
    client_manager.cpp
    argument_parser.cpp
    network_manager.cpp
    connection_handler.cpp
)
set(GRAPHICS OFF)
if(${GRAPHICS}) 
    set(CLIENT_SOURCE ${CLIENT_SOURCE} 
        main_graphics.cpp
    )
endif()
add_library(client ${CLIENT_SOURCE})
target_link_libraries(client PRIVATE base)
target_include_directories(client PUBLIC ${INCLUDE_PARAMS}) 
target_compile_options(client PUBLIC -frtti -Wall -Wextra -Wpedantic)

if(${GRAPHICS})
    qt_add_executable(p2ptransfer main_graphics.cpp)
else()
    add_executable(p2ptransfer main.cpp)
endif()
target_link_libraries(p2ptransfer PRIVATE client base ${LINKAGE_PARAMS})
