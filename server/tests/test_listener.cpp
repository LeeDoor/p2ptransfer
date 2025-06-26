#include "file_processor_mock.hpp"
#include "gtest_headers.hpp"
#include "connection_establisher_mock.hpp"
#include "listener_impl.hpp"
#include "model_mock_factory.hpp"
#include "remote_interaction_callback_mock.hpp"

class ListenerFixture : public ::testing::Test {
protected:
    ListenerFixture() :
        factory_(std::make_shared<ModelMockFactory>()),
        connection_establisher_(std::make_shared<ConnectionEstablisherMock>()),
        file_processor_(std::make_shared<FileProcessorMock>())
    {
        factory_->set_connection_establisher(connection_establisher_);
        factory_->set_file_processor(file_processor_);
        listener_ = std::make_shared<ListenerImpl>(factory_);
        listener_->set_callback(callback_);
    }
    
    std::shared_ptr<ModelMockFactory> factory_;
    std::shared_ptr<ConnectionEstablisherMock> connection_establisher_;
    std::shared_ptr<FileProcessorMock> file_processor_;
    std::shared_ptr<RemoteInteractionCallbackMock> callback_;
    std::shared_ptr<ListenerImpl> listener_;
};

TEST_F(ListenerFixture, success) {
    listener_->listen_if_not_already(TEST_PORT);
}
