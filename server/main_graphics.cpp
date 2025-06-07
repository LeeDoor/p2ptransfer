#include "gui_view.hpp"
#include "presenter.hpp"

int main(int argc, char** argv) {
    std::unique_ptr<IView> view_ptr = std::make_unique<GUIView>(argc, argv);
    Presenter presenter(std::move(view_ptr));
    return presenter.run();
}
