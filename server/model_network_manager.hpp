#pragma once

#include "port.hpp"
#include "model_callback_dependent.hpp"
class IModelNetworkManager : public ModelCallbackDependent { 
public:
    virtual ~IModelNetworkManager() = default;
    virtual int init(Port port) = 0;
};
