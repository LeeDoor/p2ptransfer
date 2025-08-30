#include "transferer_view_cli.hpp"

namespace p2ptransfer {
namespace view {

TransfererViewCLI::TransfererViewCLI(std::shared_ptr<GeneralViewCLI> view) 
    : general_view_{view}
{}
void TransfererViewCLI::setup() {
    general_view_->subscribe_transfer(
        [self = shared_from_this()](const Address& address, Port port, const Filename& filename){
        self->transfer(address, port, filename);
    });
}
void TransfererViewCLI::transfer(const Address& address, Port port, const Filename& filename) {
    Logger::log() << "Transfering file " << filename << " at " << address << ":" << port << std::endl;
    callback()->transfer(address, port, filename);
}

}
}
