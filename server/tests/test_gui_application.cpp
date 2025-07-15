#include "gui_application.hpp"

namespace general {
namespace server {
namespace test {

class GuiApplicationFixture : public ::testing::Test {
protected:
    GuiApplication application;
};
#define GEN_ARGS \
    int argc = 1; \
    char exec__[] = "aboba"; \
    char *argv[] = {exec__}

TEST_F(GuiApplicationFixture, SIGINTafterBigDelay) {
    GEN_ARGS;
    std::jthread thread{ [] {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::raise(SIGINT);
    }};
    
    application.run(argc, argv);
}

}
}
}
