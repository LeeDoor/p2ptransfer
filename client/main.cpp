#include "client_manager.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    Logger::init();
    ClientManager manager;
    return manager.start(argc, argv);
    Logger::destroy();
}
