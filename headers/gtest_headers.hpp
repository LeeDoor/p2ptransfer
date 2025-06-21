#pragma once
#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::Return;

constexpr size_t TEST_PORT = 8080;
constexpr std::string TEST_ADDRESS = "127.0.0.1";
constexpr std::string TEST_FILENAME = "test_file.txt";
const std::string TEST_FILE_CONTENT = "This is the test file@@@ content??? !!! \n\n";
const size_t TEST_FILESIZE = TEST_FILE_CONTENT.size();

template<typename Type>
net::awaitable<Type> return_immediately(Type t) {
    co_return t;
}
inline net::awaitable<void> return_immediately(){
    co_return;
}
