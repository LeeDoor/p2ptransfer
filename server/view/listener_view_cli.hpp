#pragma once
#include "general_view_cli.hpp"
#include "listener_view.hpp"

namespace general {
namespace server {
namespace view {

class ListenerViewCLI : public ListenerView {
public:
    ListenerViewCLI(std::shared_ptr<GeneralViewCLI> view);

    void stop() override;
    bool ask_file_verification(const Filename& filename, Filesize filesize) override;

private:
    void read_port_and_listen();

    std::shared_ptr<GeneralViewCLI> general_view_;
};

}
}
}
