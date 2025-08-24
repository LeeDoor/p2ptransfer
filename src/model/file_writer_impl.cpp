#include "file_writer_impl.hpp"

namespace p2ptransfer {
namespace model {

FileWriterImpl::FileWriterImpl(std::shared_ptr<SocketManager> socket) 
    : socket_{socket}
{}
net::awaitable<void> FileWriterImpl::write_file(const Filename& filename) {
    co_return;
}

}
}
