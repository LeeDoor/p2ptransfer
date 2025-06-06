set(PRECOMPILED_HEADERS 
    stdlib.hpp
    common_types.hpp
)
if(${GRAPHICS}) 
    set(PRECOMPILED_HEADERS ${PRECOMPILED_HEADERS} 
        qt_headers.hpp
    )
endif()
add_library(base INTERFACE)
target_precompile_headers(base INTERFACE ${PRECOMPILED_HEADERS})
