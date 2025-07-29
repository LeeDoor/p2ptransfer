#pragma once
#include "file_processor.hpp"
#include "general_presenter_callback.hpp"
#include "socket_manager.hpp"

namespace general {
namespace server {
namespace model {

/*!
* \brief Builder for \ref FileProcessor .
*
* Builder pattern allows avoiding object initialization at setup stage.
*/
class FileProcessorBuilder {
public:
    using CallbackPtr = std::shared_ptr<presenter::GeneralPresenterCallback>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;

    virtual ~FileProcessorBuilder() = default;

    virtual std::shared_ptr<FileProcessor> create_file_processor(
        CallbackPtr callback,
        SocketManagerPtr socket) = 0;
};

}
}
}
