#pragma once

class IAddressGatherer {
public:
    virtual ~IAddressGatherer() = default;
    virtual void gather_local_address() = 0;
};
