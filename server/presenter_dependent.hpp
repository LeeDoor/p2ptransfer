#pragma once
#include <memory>
class Presenter;
class PresenterDependent {
public:
    void set_presenter(std::shared_ptr<Presenter> presenter) {
        presenter_ = presenter;
    }
protected:
    std::weak_ptr<Presenter> presenter_;
};
