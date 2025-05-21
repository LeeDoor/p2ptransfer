#include "client_manager.hpp"

int main(int argc, char** argv) {
    ClientManager manager;
    return manager.start(argc, argv);
}
