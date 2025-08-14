#include "transferer_impl.hpp"

namespace p2ptransfer {
namespace model {

void TransfererImpl::transfer_file(const Address& address, Port port, const Filename& filename) {
    Logger::log() << "Sending file at " << filename << " to " << address << ":" << port << std::endl;
}

}
}
