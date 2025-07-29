#pragma once
#include "callback.hpp"
#include "general_presenter_callback.hpp"

namespace general {
namespace server {
namespace model {

/*!
* \brief Class for reading remote file.
*
* Reads remote file, when file gathering is agreed with user.
* This class is used by the \ref Listener.
*/
class FileProcessor : public WithCallback<presenter::GeneralPresenterCallback> {
public:
    virtual ~FileProcessor() = default;

    /*! \throws std::runtime_error if file was not gathered due
    * to network problems or user's cancellation. */
    virtual net::awaitable<void> try_read_file() = 0;
};

}
}
}
