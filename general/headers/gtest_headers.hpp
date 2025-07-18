#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

namespace general {

using ::testing::Return;

template<typename Type>
net::awaitable<Type> return_immediately(Type t) {
    co_return t;
}
inline net::awaitable<void> return_immediately(){
    co_return;
}

constexpr size_t TEST_PORT = 8080;
constexpr std::string TEST_LOCADDR = "127.0.0.1";

}
