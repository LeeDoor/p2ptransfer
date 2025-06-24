#pragma once
#include "view_factory.hpp"

class ViewGUIFactory : public ViewFactory {
public:
    ViewGUIFactory(int& argc, char** argv);
    std::shared_ptr<IView> create_view() override;

private:
    int& argc_; 
    char** argv_;
};
