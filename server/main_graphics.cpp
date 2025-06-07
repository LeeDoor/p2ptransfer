#include "gui_view.hpp"
#include "presenter.hpp"

int main(int argc, char** argv) {
    auto view_ptr = std::make_shared<GUIView>(argc, argv);
    auto presenter = std::make_shared<Presenter>(std::move(view_ptr));
    return presenter->run();
}
