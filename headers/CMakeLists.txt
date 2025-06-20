set(PRECOMPILED_HEADERS 
    stdlib.hpp
    common_types.hpp
)
if(${GRAPHICS}) 
    set(PRECOMPILED_HEADERS ${PRECOMPILED_HEADERS} 
        qt_headers.hpp
    )
endif()
add_library(base 
    request_serializer.cpp
    request_deserializer.cpp
)
target_precompile_headers(base PUBLIC ${PRECOMPILED_HEADERS})
target_link_libraries(base PUBLIC ${LINK_LIBRARIES})
