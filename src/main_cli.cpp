#include "cli_application.hpp"

int main(int argc, char** argv) {
    p2ptransfer::CLIApplication application(argc, argv);
    return application.run();
}
