#pragma once
#include "listener_presenter.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "general_presenter.hpp"
#include "transferer_impl.hpp"
#include "transferer_presenter.hpp"
#include "transferer_view.hpp"

namespace p2ptransfer {
template<typename GeneralViewType, typename ListenerViewType, typename TransfererViewType>
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
        listener_presenter_{std::make_shared<presenter::ListenerPresenter>(general_presenter_, listener_, listener_view_)},

        transferer_{std::make_shared<model::TransfererImpl>()},
        transferer_view_{std::make_shared<TransfererViewType>(general_view_)},
        transferer_presenter_{std::make_shared<presenter::TransfererPresenter>(transferer_view_, transferer_, general_presenter_)}
    {}

    int run() {
        SignalHandler::handle_SIGINT([this]() {
            general_presenter_->stop();
            listener_presenter_->stop();
            transferer_presenter_->stop();
        });
        general_presenter_->setup();
        listener_presenter_->setup();
        transferer_presenter_->setup();
        return general_presenter_->run();
    }

private:
    std::shared_ptr<GeneralViewType> general_view_;
    std::shared_ptr<presenter::GeneralPresenter> general_presenter_;

    std::shared_ptr<ListenerViewType> listener_view_;
    std::shared_ptr<model::Listener> listener_;
    std::shared_ptr<presenter::ListenerPresenter> listener_presenter_;

    std::shared_ptr<model::Transferer> transferer_;
    std::shared_ptr<view::TransfererView> transferer_view_;
    std::shared_ptr<presenter::TransfererPresenter> transferer_presenter_;
};

}
