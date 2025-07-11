#pragma once
#include "view_builder.hpp"

class ViewMockBuilder : public ViewBuilder {
public:
    ViewMockBuilder(
        std::shared_ptr<View> view
    ) : 
        view_(view)
{}

    std::shared_ptr<View> create_view() override;
private:
    std::shared_ptr<View> view_;
};
