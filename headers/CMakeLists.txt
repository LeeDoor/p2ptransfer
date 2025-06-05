add_library(base INTERFACE)
target_precompile_headers(base INTERFACE
    stdlib.hpp
    address.hpp
    logger.hpp
    common_types.hpp
    port.hpp
    request_serializer.hpp
    request_deserializer.hpp
    request_header_names.hpp
    request_structures.hpp
)
