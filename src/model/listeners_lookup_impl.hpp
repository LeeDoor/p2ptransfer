#pragma once
#include "listeners_lookup.hpp"

namespace p2ptransfer {

class SocketManagerMulticast;
class SocketManagerBuilder;
class ThreadWrapper;

namespace model {

class ListenersLookupImpl : public ListenersLookup {
public:
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerPtr = std::shared_ptr<SocketManagerMulticast>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;

    virtual ~ListenersLookupImpl() = default;
 
    ListenersLookupImpl(
        ContextPtr context,
        SocketManagerBuilderPtr socket_builder,
        ThreadWrapperPtr thread_wrapper);

    void start_lookup() override;
    void stop() override;

private:
    static constexpr std::string LOOKUP_MSG = "Stub message";
    static constexpr Address LOOKUP_ADDRESS = "233.255.0.1";
    static constexpr Port LOOKUP_PORT = 45953;

    void run_lookup();
    net::awaitable<void> lookup_async();

    SocketManagerBuilderPtr socket_builder_;
    SocketManagerPtr socket_manager_;
    ThreadWrapperPtr thread_wrapper_;
    ContextPtr context_;
};

}
}
