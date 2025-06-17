#pragma once

class IUICallback {
public:
    virtual ~IUICallback() = default;
    virtual void listen() = 0;
};
