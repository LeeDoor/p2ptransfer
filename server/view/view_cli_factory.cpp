#include "view_cli_factory.hpp"
#include "view_cli.hpp"

std::shared_ptr<View> ViewCLIFactory::create_view() {
    return std::make_shared<ViewCLI>();
}
