#pragma once

#include "transferer.hpp"
namespace p2ptransfer {
namespace model {

class TransfererImpl : public Transferer {
public:
    void transfer_file(const Address& address, Port port, const Filename& filename) override;
};

}
}
