#pragma once
#include "transferer_view.hpp"

namespace p2ptransfer {
namespace view {

class GeneralViewCLI;

class TransfererViewCLI : public TransfererView, public std::enable_shared_from_this<TransfererViewCLI> {
public:
    TransfererViewCLI(std::shared_ptr<GeneralViewCLI> view);

    void setup() override;
private:
    void transfer(const Address& address, Port port, const Filename& filename);

    std::shared_ptr<GeneralViewCLI> general_view_;
};

}
}
