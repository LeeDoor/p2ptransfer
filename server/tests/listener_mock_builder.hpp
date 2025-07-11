#pragma once

#include "listener_builder.hpp"
class ListenerMockBuilder : public ListenerBuilder {
public:
    ListenerMockBuilder (std::shared_ptr<Listener> mock) 
    : mock_(mock) {}
    std::shared_ptr<Listener> create_listener() override {
        return mock_;
    }
private:
    std::shared_ptr<Listener> mock_;
};
