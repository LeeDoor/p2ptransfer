#include "transferer_impl.hpp"

namespace p2ptransfer {
namespace model {

void TransfererImpl::stop_impl() {
}
void TransfererImpl::transfer_file(const Address& address, Port port, const Filename& filename) {
    Logger::log() << "Sending file at " << filename << " to " << address << ":" << port << std::endl;
    callback()->file_transfered();
}

}
}
