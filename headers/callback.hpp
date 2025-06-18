#pragma once

template<typename CallbackType>
class WithCallback {
public:
    void set_callback(std::shared_ptr<CallbackType> callback) {
        callback_ = callback;
    }
protected:
    std::weak_ptr<CallbackType> callback_;
};
