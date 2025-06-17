#pragma once

template<typename CallbackType>
class WithCallback {
public:
    void set_callback(CallbackType callback) {
        callback_ = std::move(callback);
    }
protected:
    CallbackType callback_;
};
