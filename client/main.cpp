#include "client_manager.hpp"
#include "logger.hpp"

int main(int argc, char** argv) {
    LoggerInitializer logger;
    ClientManager manager;
    return manager.start(argc, argv);
}
