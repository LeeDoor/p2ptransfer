#pragma once
#include "callback.hpp"
#include "transferer_view_callback.hpp"

namespace p2ptransfer {
namespace view {

class TransfererView : public WithCallback<TransfererViewCallback> {
public:
    virtual ~TransfererView() = default;

    virtual void setup() = 0;
};

}
}
