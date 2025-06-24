#include "logger.hpp"
#include "presenter_impl.hpp"
#include "view_factory_impl.hpp"
#include "model_impl_factory.hpp"

int main(int argc, char** argv) {
    LoggerInitializer init;
    auto presenter = std::make_shared<PresenterImpl>(
        std::make_shared<ModelImplFactory>(),
        std::make_shared<ViewGUIFactory>(argc, argv)
    );
    presenter->setup();
    return presenter->run();
}
