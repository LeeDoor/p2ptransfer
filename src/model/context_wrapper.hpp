#pragma once

namespace p2ptransfer {

class ContextWrapper {
public:
    ContextWrapper() 
        : context_{new net::io_context(), ContextWrapperDeleter{}}
    {}

    net::io_context* operator->() {
        return context_.get();
    }
    net::io_context& operator*() {
        return *context_;
    }
    const net::io_context& operator*() const {
        return *context_; 
    }

private:
    struct ContextWrapperDeleter {
        void operator()(net::io_context* context) const {
            // context->stop();
            delete context;
        }
    };

    std::shared_ptr<net::io_context> context_;
};

}
