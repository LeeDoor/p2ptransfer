#include "gui_application.hpp"

int main(int argc, char** argv) {
    general::server::GuiApplication application;
    return application.run(argc, argv);
}
