#include "server_manager.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    Logger::init();
    ServerManager manager;
    return manager.start(argc, argv);
    Logger::destroy();
}
