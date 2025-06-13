#pragma once

#include "presenter_dependent.hpp"
class IModelAddressGatherer : public PresenterDependent {
public:
    virtual void gather_local_address() = 0;
};
