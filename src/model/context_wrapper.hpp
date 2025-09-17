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
        if(context_)
            context_->stop();
    }

    net::io_context* operator->() {
        return context_.get();
    }
    net::io_context& operator*() {
        return *context_;
    }
    const net::io_context& operator*() const {
        auto& a = *context_;
        std::cout << "*context_: " << &a << std::endl;
        return a; 
    }
private:
    std::shared_ptr<net::io_context> context_;
};

}
