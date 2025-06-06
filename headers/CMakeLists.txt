set(PRECOMPILED_HEADERS 
    stdlib.hpp
    common_types.hpp
    address.hpp
    logger.hpp
    port.hpp
    request_serializer.hpp
    request_deserializer.hpp
    request_header_names.hpp
    request_structures.hpp
)
if(${GRAPHICS}) 
    set(PRECOMPILED_HEADERS ${PRECOMPILED_HEADERS} 
        #qt_headers.hpp
    )
endif()
add_library(base INTERFACE)
target_precompile_headers(base INTERFACE ${PRECOMPILED_HEADERS})
