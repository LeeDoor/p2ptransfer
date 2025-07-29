#pragma once

namespace general {

class Stoppable {
public:
    virtual ~Stoppable() = default;

    virtual void stop() = 0;
};

}
