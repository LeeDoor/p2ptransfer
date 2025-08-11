#pragma once
#include "logger.hpp"

namespace general {

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
            std::cerr << Logger::ERROR_COLOR << "Callback is not set to somebody: " 
                << Logger::CLEAR_COLOR << std::endl
                << boost::stacktrace::stacktrace() << std::endl;;
#ifdef TESTING
            // Forcing tests to fail if forgot to initialize the callback
            std::terminate();
#endif
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
        throw std::logic_error("callback is dead");
    }

    std::weak_ptr<CallbackType> callback_;
    bool initialized_;
};

}
