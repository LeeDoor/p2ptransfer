#pragma once
#include "listener_presenter.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "general_presenter.hpp"

namespace general {
namespace server {

template<typename GeneralViewType, typename ListenerViewType>
class Application {
public:
    template<typename GeneralViewGenerator>
    Application(GeneralViewGenerator&& gvg) :
        general_view_{std::make_shared<GeneralViewType>(gvg())},
        general_presenter_{std::make_shared<presenter::GeneralPresenter>(
            model::ModelBuilder::create_address_gatherer(),
            general_view_
        )},
        listener_view_{std::make_shared<ListenerViewType>(general_view_)},
        listener_{model::ModelBuilder::create_listener()},
        listener_presenter_{std::make_shared<presenter::ListenerPresenter>(listener_, listener_view_)}
    {}

    int run() {
        SignalHandler::handle_SIGINT([this]() {
            general_presenter_->stop();
            listener_presenter_->stop();
        });
        general_presenter_->setup();
        listener_presenter_->setup();
        return general_presenter_->run();
    }

private:
    std::shared_ptr<GeneralViewType> general_view_;
    std::shared_ptr<presenter::GeneralPresenter> general_presenter_;

    std::shared_ptr<ListenerViewType> listener_view_;
    std::shared_ptr<model::Listener> listener_;
    std::shared_ptr<presenter::ListenerPresenter> listener_presenter_;
};

}
}
