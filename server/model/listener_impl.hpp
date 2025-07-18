#pragma once
#include "file_processor_builder.hpp"
#include "listener.hpp"
#include "socket_manager_builder.hpp"
#include "thread_wrapper.hpp"

namespace general {
namespace server {
namespace model {

/*!
* \brief Socket implementation for \ref Listener .
* Implements network integration on separate thread using \ref ThreadWrapper .
* This class performs headers exchange, such as SendRequest and SendPermission.
* File sharing responsibility is passed to \ref FileProcessor .
*/
class ListenerImpl : public Listener {
public:
    using ThreadWrapperPtr = std::shared_ptr<ThreadWrapper>;
    using SocketManagerBuilderPtr = std::shared_ptr<SocketManagerBuilder>;
    using SocketManagerPtr = std::shared_ptr<SocketManager>;
    using FileProcessorBuilderPtr = std::shared_ptr<FileProcessorBuilder>;
    using ContextPtr = std::shared_ptr<net::io_context>;

    /*!
    * \param context Shared pointer to boost::asio::io_context.
    * \param thread_wrapper pointer to \ref ThreadWrapper for detaching
    * network communication to other thread.
    * \param socket_manager_builder pointer to \ref SocketManagerBuilder .
    * Required to create socket at runtime, scince it's lifetime is tied
    * with connection duration.
    * \param file_processor_builder pointer to \ref FileProcessorBuilder .
    * Required to create FileProcessor at runtime. Used only when file 
    * sharing is accepted by user.
    */
    ListenerImpl(ContextPtr context,
                 ThreadWrapperPtr thread_wrapper,
                 SocketManagerBuilderPtr socket_manager_builder,
                 FileProcessorBuilderPtr file_processor_builder);
    void listen_if_not_already(Port port) override;
    void stop() override;

private:
    void spawn_listen_coroutine(Port port);
    net::awaitable<void> listen_async(Port port);
    void spawn_and_run(Port port);
    net::awaitable<SocketManagerPtr> connect_and_listen(Port port);
    net::awaitable<SocketManagerPtr> build_socket_manager(Port port);

    ThreadWrapperPtr thread_wrapper_;
    SocketManagerBuilderPtr socket_manager_builder_;
    FileProcessorBuilderPtr file_processor_builder_;
    ContextPtr context_;
};

}
}
}
