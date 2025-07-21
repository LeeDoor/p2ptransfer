#include "view_gui.hpp"
#include "client_presenter.hpp"

int main(int argc, char** argv) {
    using namespace general::client;
    auto application = std::make_shared<QApplication>(argc, argv);
    auto presenter = std::make_shared<presenter::ServerPresenter>(
        std::make_shared<view::ViewGUI>(application)
    );
    presenter->setup();
    return presenter->run();
}
