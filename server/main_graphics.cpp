#include "gui_application.hpp"

int main(int argc, char** argv) {
    general::server::GuiApplication application(argc, argv);
    return application.run();
}
