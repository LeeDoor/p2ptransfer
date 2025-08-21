#pragma once
#include "network_status_callback.hpp"
#include "request_structures.hpp"

namespace p2ptransfer {
namespace model {

class FileWriter : public WithNetworkCallback {
public:
    virtual ~FileWriter() = default;

    virtual void write_file(const Filename& filename) = 0;
};

}
}
