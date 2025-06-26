#pragma once
#include "view_factory.hpp"

class ViewMockFactory : public ViewFactory {
public:
    ViewMockFactory(
        std::shared_ptr<View> view
    ) : 
        view_(view)
{}

    std::shared_ptr<View> create_view() override;
private:
    std::shared_ptr<View> view_;
};
