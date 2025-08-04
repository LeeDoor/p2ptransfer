#include "gui_application.hpp"

int main(int argc, char** argv) {
    general::server::GUIApplication application(argc, argv);
    return application.run();
}
