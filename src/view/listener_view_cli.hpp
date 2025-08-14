#pragma once
#include "general_view_cli.hpp"
#include "listener_view.hpp"

namespace p2ptransfer {
namespace view {

class ListenerViewCLI : public ListenerView, public std::enable_shared_from_this<ListenerViewCLI> {
public:
    ListenerViewCLI(std::shared_ptr<GeneralViewCLI> view);

    void setup() override;
    bool ask_file_verification(const Filename& filename, Filesize filesize) override;

private:
    void read_port_and_listen();

    std::shared_ptr<GeneralViewCLI> general_view_;
};

}
}
