#include "cli_application.hpp"

int main() {
    p2ptransfer::server::CLIApplication application;
    return application.run();
}
