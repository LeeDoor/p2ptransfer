#pragma once
#include "model_callback.hpp"
#include <memory>
class Presenter;
class ModelCallbackDependent {
public:
    void set_model_callback(std::shared_ptr<IModelCallback> callback) {
        callback_ = callback;
    }
protected:
    std::weak_ptr<IModelCallback> callback_;
};
