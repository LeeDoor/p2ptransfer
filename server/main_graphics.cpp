#include "presenter.hpp"
#include "logger.hpp"
#include "presenter_builder.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    PresenterBuilder presenter_builder;
    std::shared_ptr<Presenter> presenter = presenter_builder.build_GUI(argc, argv);
    return presenter->run();
}
