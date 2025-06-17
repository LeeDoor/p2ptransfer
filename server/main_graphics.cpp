#include "presenter.hpp"
#include "logger.hpp"
#include "gui_presenter_builder.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    GUIPresenterBuilder presenter_builder(argc, argv);
    std::shared_ptr<Presenter> presenter = presenter_builder.build();
    return presenter->run();
}
