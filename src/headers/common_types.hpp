#pragma once
#define BOOST_ASIO_NO_DEPRECATED

namespace boost { namespace asio { namespace ip { class tcp; } } };
namespace net = boost::asio;
using tcpip = net::ip::tcp;
using ErrorCode = boost::system::error_code;

struct ContextPtr {
    ContextPtr() 
    : context_{std::make_shared<net::io_context>()}
    {}

    net::io_context* operator->() {
        return context_.get();
    }
    const net::io_context& operator*() const {
        return *context_;
    }
    net::io_context& operator*() {
        return *context_;
    }

    std::shared_ptr<net::io_context> context_;
};
