#pragma once
#include "file_writer.hpp"
#include "socket_manager.hpp"

namespace p2ptransfer {
namespace model {

class FileWriterBuilder {
public:
    virtual std::shared_ptr<FileWriter> create_file_writer(
        WithNetworkCallback::Callback network_callback,
        std::shared_ptr<SocketManager> socket) = 0;
};

}
}
