#pragma once
#include "address.hpp"
#include "request_structures.hpp"
#include "view.hpp"

namespace general {
namespace client {
namespace presenter {

class Presenter : public view::ViewCallback,
                  public std::enable_shared_from_this<Presenter> {
public:
    using ViewPtr = std::shared_ptr<view::View>;

    Presenter(ViewPtr view);

    void setup();
    int run();
    void stop();

    void send_file_to(Filename filename, Address address, Port port) override;

private:
    ViewPtr view_;
};

}
}
}
