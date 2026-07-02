#pragma once
#include "lookup_results_callback.hpp"

namespace p2ptransfer {
namespace model {

/*!
* \brief Part of Model of MVP.
*
* Sends broadcast messages to everyone in LAN to detect
* listening users around.
*/
class ListenersLookup : public WithLookupResultsCallback {
public:
    virtual ~ListenersLookup() = default;
 
    /*! Listens for user connection and starts communication.
    * \throws std::runtime_error if file was not gathered due to network 
    * problems or user cancellation.
    */
    virtual void start_lookup() = 0;
    virtual void stop() = 0;
};

}
}
