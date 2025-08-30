#pragma once
#include "network_status_callback.hpp"

namespace p2ptransfer {

class SocketManager;

namespace model {

class FileWriter;

class FileWriterBuilder {
public:
    virtual std::shared_ptr<FileWriter> create_file_writer(
        WithNetworkCallback::Callback network_callback,
        std::shared_ptr<SocketManager> socket) = 0;
};

}
}
