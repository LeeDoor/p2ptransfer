#pragma once

namespace p2ptransfer {
namespace view {

class LookupViewCallback {
public:
    virtual ~LookupViewCallback() = default;

    virtual void lookup() = 0;
    virtual void cancel_lookup() = 0;
};

}
}
