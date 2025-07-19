#include "presenter.hpp"

namespace general {
namespace client {
namespace presenter {

Presenter::Presenter(ViewPtr view) 
    : view_(view)
{}

void Presenter::setup() {
    view_->set_callback(shared_from_this());
}
int Presenter::run() {
    return view_->run();
}
void Presenter::stop() {
    view_->stop();
}

void Presenter::send_file_to(Filename filename, Address address, Port port) {
    std::cout << "send_file_to call " + filename + " to " + address + ":" + std::to_string(port) << std::endl;
}

}
}
}
