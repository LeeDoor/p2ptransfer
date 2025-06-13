#pragma once
#include "view_callback.hpp"
#include <memory>
class Presenter;
class ViewCallbackDependent {
public:
    virtual ~ViewCallbackDependent() = default;
    void set_view_callback(std::shared_ptr<IViewCallback> callback) {
        callback_ = callback;
    }
protected:
    std::weak_ptr<IViewCallback> callback_;
};
