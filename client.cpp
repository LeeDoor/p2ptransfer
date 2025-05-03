#include <filesystem>
#include <iostream>
#include <array>
#include <fstream>
#include <boost/asio.hpp>
using namespace boost::asio;
using namespace boost::asio::ip;

io_context context;
tcp::resolver resolver{context};
tcp::socket tcp_socket{context};
std::ifstream file;
uintmax_t bytes_to_send;

std::array<char, 4096> data;

void write_handler(const boost::system::error_code& ec, std::size_t transfered) {
    if(ec) {
        std::cout << "failed to write data: " << ec.what();
        file.close();
        return;
    }
    std::cout << "transfered: " << transfered << std::endl;
    bytes_to_send -= transfered;
    if(bytes_to_send == 0) {
        std::cout << "finished sending." << std::endl;
        tcp_socket.shutdown(tcp::socket::shutdown_send);
        tcp_socket.close();
        file.close();
        return;
    }
    file.read(data.data(), std::min(4096lu, bytes_to_send));
    tcp_socket.async_write_some(buffer(data, std::min(4096lu, bytes_to_send)), write_handler);
}

void connect_handler(const boost::system::error_code& ec) {
    if(ec) {
        std::cout << "failed to connect: " << ec.what() << std::endl;
        return;
    }
    std::cout << "connected." << std::endl;
    file.open("input.txt", std::ios::in | std::ios::binary);
    file.read(data.data(), std::min(4096lu, bytes_to_send));
    tcp_socket.async_write_some(buffer(data, std::min(4096lu, bytes_to_send)), write_handler);
}

void resolve_handler(const boost::system::error_code& ec, tcp::resolver::results_type result) {
    if(ec) {
        std::cout << "failed to resolve data: " << ec.what() << std::endl;
        return;
    }
    tcp_socket.async_connect(*result.begin(), connect_handler);
}

int main() {
    if(!std::filesystem::exists("input.txt")) {
        std::cout << "input file does not exists." << std::endl;
        return 1;
    }
    bytes_to_send = std::filesystem::file_size("input.txt");
    std::cout << "bytes to send: " << bytes_to_send << std::endl;
    if(bytes_to_send == 0) {
        std::cout << "empty input file." << std::endl;
        return 2;
    }
    resolver.async_resolve("localhost", "8080", resolve_handler);
    context.run();
}
