#pragma once
#include "network_status_callback.hpp"
#include "request_structures.hpp"

namespace p2ptransfer {
namespace model {

class Transferer : public WithNetworkCallback {
public:
    virtual ~Transferer() = default;

    virtual void transfer_file(const Address& address, Port port, const Filename& filename) = 0;
};

}
}
