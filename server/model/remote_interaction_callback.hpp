#pragma once
#include "connection_status_callback.hpp"
#include "file_transfer_callback.hpp"

namespace general {
namespace server {
namespace model {

class RemoteInteractionCallback :
    public ConnectionStatusCallback,
    public FileTransferCallback
{
public:
    virtual ~RemoteInteractionCallback() = default;
};

}
}
}
