#include "server_manager.hpp"

int main(int argc, char** argv) {
    ServerManager manager;
    return manager.start(argc, argv);
}
