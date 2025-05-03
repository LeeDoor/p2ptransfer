#include <boost/asio.hpp>
#include <iostream>
#include <array>
#include <fstream>

using namespace boost::asio::ip;
using namespace boost::asio;

io_context context;
tcp::endpoint endpoint{tcp::v4(), 8080};
tcp::acceptor acceptor{context, endpoint};
tcp::socket tcp_socket{context};
std::array<char, 4096> data;
std::ofstream file;

void accept_handler(const boost::system::error_code& ec);
void read_handler(const boost::system::error_code& ec,
                  size_t bytes_transfered) {
    if(!ec) {
        std::cout << "gathered " << bytes_transfered << " bytes" << std::endl;
        file.write(data.data(), bytes_transfered);
        tcp_socket.async_read_some(buffer(data), read_handler);
    } else {
        std::cout << "transfer finished: " << ec.what() << std::endl;
        tcp_socket.close();
        file.close();
        acceptor.async_accept(tcp_socket, accept_handler);
    }
}

void accept_handler(const boost::system::error_code& ec) {
    if(!ec) {
        std::cout << "client connected: " 
            << tcp_socket.local_endpoint().address().to_string() 
            << ":" << tcp_socket.local_endpoint().port() << std::endl;
        file.open("output.txt");
        if(!file.is_open()) {
            std::cout << "failed to open output.txt" << std::endl;
            return;
        }
        tcp_socket.async_read_some(buffer(data), read_handler);
    } else {
        std::cout << "failed while accepting connection: " << ec.what() << std::endl;
    }
}

int main() {
    acceptor.listen();
    std::cout << "server started. listening..." << std::endl;
    acceptor.async_accept(tcp_socket, accept_handler);
    context.run();
}
