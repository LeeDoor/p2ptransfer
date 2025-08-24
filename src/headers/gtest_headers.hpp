#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace p2ptransfer {

using ::testing::Return;
using ::testing::AtLeast;

template<typename Type>
net::awaitable<Type> return_immediately(Type t) {
    co_return t;
}
inline net::awaitable<void> return_immediately(){
    co_return;
}

constexpr size_t TEST_PORT = 8080;
constexpr std::string TEST_LOCADDR = "127.0.0.1";
constexpr std::string TEST_FILENAME = "file.txt";

}
