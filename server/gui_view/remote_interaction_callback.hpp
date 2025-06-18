#pragma once

#include "connection_status_callback.hpp"
#include "file_transfer_callback.hpp"
class IRemoteInteractionCallback :
    public IConnectionStatusCallback,
    public IFileTransferCallback
{
public:
    virtual ~IRemoteInteractionCallback() = default;
};
