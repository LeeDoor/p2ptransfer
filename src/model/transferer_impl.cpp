#include "transferer_impl.hpp"

namespace p2ptransfer {
namespace model {

TransfererImpl::TransfererImpl(
    ContextPtr context,
    ThreadWrapperPtr thread_wrapper,
    SocketManagerBuilderPtr socket_builder)
    : context_{context}
    , thread_wrapper_{thread_wrapper}
    , socket_builder_{socket_builder}
{}

void TransfererImpl::transfer_file(const Address& address, Port port, const Filename& filename) {
    if(!thread_wrapper_->is_running()) {
        thread_wrapper_->execute([&] {
            Logger::log() << "Sending file at " << filename << " to " << address << ":" << port << std::endl;
        });
    }
}

}
}
