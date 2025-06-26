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
    add_library(test_headers INTERFACE)
    target_precompile_headers(test_headers INTERFACE headers/gtest_headers.hpp)
    add_subdirectory(tests)
endif()
add_library(base 
    serializer/request_serializer.cpp
    serializer/request_deserializer.cpp
    string_view/string_view_methods.cpp
    #thread_wrapper/thread_wrapper_impl.cpp
)
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} headers serializer string_view)
target_precompile_headers(base PUBLIC ${PRECOMPILED_HEADERS})
target_link_libraries(base PUBLIC ${LINK_LIBRARIES})
target_include_directories(base PUBLIC ${INCLUDE_DIRECTORIES})
set(INCLUDE_DIRECTORIES ${INCLUDE_DIRECTORIES} PARENT_SCOPE)
