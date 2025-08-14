#pragma once
#include "request_structures.hpp"

namespace p2ptransfer {
namespace server {
namespace presenter {

/// The union of \ref ConnectionStatusCallback and \ref FileTransferCallback.
class ListenerCallback {
public:
    virtual ~ListenerCallback() = default;

    /// Asks for user input about file gathering confirmation.
    virtual bool verify_file(const Filename& filename, Filesize filesize) = 0;
};

}
}
}
