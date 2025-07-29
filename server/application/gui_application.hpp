#pragma once
#include "general_presenter.hpp"
#include "listener_presenter.hpp"
#include "listener_view_gui.hpp"
#include "model_builder.hpp"
#include "signal_handler.hpp"
#include "general_view_gui.hpp"

namespace general {
namespace server {

class GuiApplication {
public:
    GuiApplication(int argc, char** argv) :
        application_{std::make_shared<QApplication>(argc, argv)},
        general_view_{std::make_shared<view::GeneralViewGUI>(application_)},
        general_presenter_{std::make_shared<presenter::GeneralPresenter>(
            model::ModelBuilder::create_address_gatherer(),
            general_view_
        )},
        listener_view_{std::make_shared<view::ListenerViewGUI>(general_view_)},
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
    std::shared_ptr<QApplication> application_;
    std::shared_ptr<view::GeneralViewGUI> general_view_;
    std::shared_ptr<presenter::GeneralPresenter> general_presenter_;

    std::shared_ptr<view::ListenerViewGUI> listener_view_;
    std::shared_ptr<model::Listener> listener_;
    std::shared_ptr<presenter::ListenerPresenter> listener_presenter_;
};

}
}
