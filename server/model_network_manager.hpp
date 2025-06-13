#pragma once

#include "port.hpp"
#include "presenter_dependent.hpp"
class IModelNetworkManager : public PresenterDependent { 
public:
    virtual int init(Port port) = 0;
};
