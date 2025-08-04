#pragma once

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
        if(!initialized_)
            std::cerr << "\n\nWARNING: Callback is not set to somebody. Check the stack trace.\n\n";
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
