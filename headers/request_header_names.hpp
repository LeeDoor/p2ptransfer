#pragma once
#include "stdlib.hpp"
using HeaderType = std::string_view;
// generates constexpr string and calculates it's size at compile time.
// size is stored in name_SIZE variable.
#define MAKE_CONSTEXPR(name, value) \
constexpr HeaderType name = value; \
constexpr size_t name ## _SIZE = name.size();
MAKE_CONSTEXPR(REQUEST_HEADER, "REQUEST")
MAKE_CONSTEXPR(FILE_HEADER, "FILE")
MAKE_CONSTEXPR(SIZE_HEADER, "SIZE")
MAKE_CONSTEXPR(PERMISSION_HEADER, "PERMISSION")
