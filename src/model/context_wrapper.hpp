#pragma once

namespace p2ptransfer {

class ContextWrapper {
public:
    ContextWrapper() 
        : context_{std::make_shared<net::io_context>()}
    {}
    ContextWrapper(const ContextWrapper& other) = default;
    ContextWrapper(ContextWrapper&& other) = default;
    ContextWrapper& operator=(const ContextWrapper& other) = default;
    ContextWrapper& operator=(ContextWrapper&& other) = default;
    ~ContextWrapper() {
        context_->stop();
    }

    net::io_context* operator->() {
        return context_.get();
    }
    net::io_context& operator*() {
        return *context_;
    }
private:
    std::shared_ptr<net::io_context> context_;
};

}
