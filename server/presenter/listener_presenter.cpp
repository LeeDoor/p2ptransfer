#include "listener_presenter.hpp"

namespace general {
namespace server {
namespace presenter {

ListenerPresenter::ListenerPresenter(std::shared_ptr<GeneralPresenter> general_presenter)
    : general_presenter_{general_presenter}
{}

void ListenerPresenter::listen(Port port) {
    
}

}
}
}
