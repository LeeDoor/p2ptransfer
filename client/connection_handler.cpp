#include "connection_handler.hpp"
#include <iostream>

int ConnectionHandler::handle() {
    std::cout << "handling connection on " << socket_.remote_endpoint().address() << ":" << socket_.remote_endpoint().port() << std::endl;
    socket_.shutdown(tcpip::socket::shutdown_both);
    socket_.close();
    return 0;
}
