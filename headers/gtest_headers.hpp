#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;

template<typename Type>
net::awaitable<Type> return_immediately(Type t) {
    co_return t;
}
inline net::awaitable<void> return_immediately(){
    co_return;
}
