#pragma once

#include "presenter_dependent.hpp"
class IModelAddressGather : public PresenterDependent {
public:
    virtual void gather_local_address() = 0;
};
