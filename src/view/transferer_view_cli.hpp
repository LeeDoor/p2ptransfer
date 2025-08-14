#pragma once
#include "general_view_cli.hpp"
#include "transferer_view.hpp"

namespace p2ptransfer {
namespace view {

class TransfererViewCLI : public TransfererView, public std::enable_shared_from_this<TransfererViewCLI> {
public:
    TransfererViewCLI(std::shared_ptr<GeneralViewCLI> view);

    void setup() override;
private:
    void read_data_and_transfer();

    std::shared_ptr<GeneralViewCLI> general_view_;
};

}
}
