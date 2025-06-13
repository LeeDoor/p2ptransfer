#pragma once
#include "test_headers.hpp"
class Interface {
public:
    virtual ~Interface() = default;
    virtual int add(int x, int y) = 0;
};
class MockInterface : Interface {
public:
    MOCK_METHOD(int, add, (int x, int y), (override));
};
