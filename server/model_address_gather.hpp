#pragma once

#include "model_callback_dependent.hpp"
class IModelAddressGatherer : public ModelCallbackDependent {
public:
    virtual void gather_local_address() = 0;
};
