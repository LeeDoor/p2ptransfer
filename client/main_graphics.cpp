#include "view_gui.hpp"
#include "presenter.hpp"

int main(int argc, char** argv) {
    using namespace general::client;
    auto application = std::make_shared<QApplication>(argc, argv);
    auto presenter = std::make_shared<presenter::Presenter>(
        std::make_shared<view::ViewGUI>(application)
    );
    presenter->setup();
    return presenter->run();
}
