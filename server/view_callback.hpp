#pragma once

class IViewCallback {
public:
    virtual ~IViewCallback() = default;
    virtual void listen() = 0;
};
