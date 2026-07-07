#pragma once
#include "address.hpp"
#include "port.hpp"

namespace p2ptransfer {
namespace model {

static constexpr Address LOOKUP_ADDRESS = "233.255.0.1";
static constexpr Port LOOKUP_PORT = 45953;
static constexpr std::chrono::duration LOOKUP_PERIOD = std::chrono::seconds(1);

}
}
