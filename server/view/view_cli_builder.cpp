#include "view_cli_builder.hpp"
#include "view_cli.hpp"

std::shared_ptr<View> ViewCLIBuilder::create_view() {
    return std::make_shared<ViewCLI>();
}
