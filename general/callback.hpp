#pragma once

template<typename CallbackType>
class WithCallback {
public:
    void set_callback(std::shared_ptr<CallbackType> callback) {
        callback_ = callback;
    }
protected:
    std::shared_ptr<CallbackType> callback() {
        if(auto callback = callback_.lock())
            return callback;
        throw std::logic_error("callback is dead");
    }
    std::weak_ptr<CallbackType> callback_;
};
