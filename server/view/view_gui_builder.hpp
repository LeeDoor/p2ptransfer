#pragma once
#include "view_builder.hpp"

class ViewGUIBuilder : public ViewBuilder {
public:
    ViewGUIBuilder(int& argc, char** argv);
    std::shared_ptr<View> create_view() override;

private:
    int& argc_; 
    char** argv_;
};
