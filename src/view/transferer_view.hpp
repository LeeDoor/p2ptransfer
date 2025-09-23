#pragma once
#include "callback.hpp"
#include "transferer_view_callback.hpp"

namespace p2ptransfer {
namespace view {

/// brief Transferer View part of MVP
class TransfererView : public WithCallback<TransfererViewCallback> {
public:
    virtual ~TransfererView() = default;

    virtual void setup() = 0;
};

}
}
