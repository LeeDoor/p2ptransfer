if(${GRAPHICS})
    add_library(pch_graphics INTERFACE)
    target_precompile_headers(pch_graphics INTERFACE
        headers/qt_headers.hpp
        headers/stdlib.hpp
        headers/common_types.hpp
    )
    target_include_directories(pch_graphics INTERFACE ${INCLUDE_DIRECTORIES} ${Qt6Widgets_INCLUDE_DIRS} ${Qt6Core_INCLUDE_DIRS})
endif()
add_library(pch_base INTERFACE)
target_precompile_headers(pch_base INTERFACE 
    headers/stdlib.hpp
    headers/common_types.hpp
)
target_include_directories(pch_base INTERFACE ${INCLUDE_DIRECTORIES})
if(${TESTING})
    add_library(pch_tests INTERFACE)
    target_precompile_headers(pch_tests INTERFACE
        headers/gtest_headers.hpp
    )
endif()


