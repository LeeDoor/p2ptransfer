#include "transferer_view_cli.hpp"

namespace p2ptransfer {
namespace view {

TransfererViewCLI::TransfererViewCLI(std::shared_ptr<GeneralViewCLI> view) 
    : general_view_{view}
{}
void TransfererViewCLI::setup() {
    general_view_->subscribe_transfer([self = shared_from_this()]{
        self->read_data_and_transfer();
    });
}
void TransfererViewCLI::read_data_and_transfer() {
    callback()->transfer("1488.228.322.288", 8182, "FILE SMERTI");
}

}
}
