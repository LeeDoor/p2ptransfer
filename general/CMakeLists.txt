set(PRECOMPILED_HEADERS 
    headers/stdlib.hpp
    headers/common_types.hpp
)
if(${GRAPHICS}) 
    set(PRECOMPILED_HEADERS ${PRECOMPILED_HEADERS} 
        headers/qt_headers.hpp
    )
endif()

if(${TESTING}) 
    set(PRECOMPILED_HEADERS ${PRECOMPILED_HEADERS} 
        headers/gtest_headers.hpp
    )
endif()

add_library(base 
    serializer/request_serializer.cpp
    serializer/request_deserializer.cpp
    string_view/string_view_methods.cpp
    thread_wrapper/thread_wrapper_impl.cpp
    signal_handler/signal_handler.cpp
    address_gatherer/address_gatherer_impl.cpp
)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} 
    headers 
    serializer 
    string_view 
    thread_wrapper
    signal_handler
    address_gatherer
    socket_manager
)
target_precompile_headers(base PUBLIC ${PRECOMPILED_HEADERS})
target_link_libraries(base PUBLIC ${LINK_LIBRARIES})
target_include_directories(base PUBLIC ${INCLUDE_DIRECTORIES})
if(${TESTING})
    add_subdirectory(tests)
endif()
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)

