#pragma once
#include "logger.hpp"

namespace p2ptransfer {

/*!
* \brief provides logic for keeping and managing callback type.
*
* All children from that class should have their callbacks installed.
* Stored callback type is weak_ptr, so it will not create issues with
* memory leaks.
*/
template<typename CallbackType>
class WithCallback {
public:
    using Callback = std::shared_ptr<CallbackType>;
    WithCallback() :
        callback_{},
        initialized_{false}
    {}
    virtual ~WithCallback() {
        if(!initialized_) {
            Logger::log_stacktrace("Callback is not set here:");
        }
    }

    /// Should be called before usage
    void set_callback(std::shared_ptr<CallbackType> callback) {
        callback_ = callback;
        initialized_ = true;
    }

protected:
    /// Callback accessor.
    /*! \throws std::logic_error if callback initial shared pointer is dead. */
    std::shared_ptr<CallbackType> callback() {
        if(auto callback = callback_.lock())
            return callback;
        Logger::log_stacktrace("Callback is used before setting:");
        throw std::logic_error("callback is not set");
    }

    /// Although this member is protected and not private, access with \ref callback()
    std::weak_ptr<CallbackType> callback_;
private:
    bool initialized_;
};

}
