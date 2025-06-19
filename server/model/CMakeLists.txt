add_library(server_model
    address_gatherer.cpp
    file_processor.cpp
    connection_establisher.cpp
)
target_link_libraries(server_model PUBLIC base)
target_include_directories(server_model PUBLIC ${INCLUDE_DIRECTORIES})

if(${TESTING})
    add_subdirectory(tests)
endif()
