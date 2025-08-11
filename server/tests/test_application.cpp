#ifdef GRAPHICS
#include "gui_application.hpp"
#else
#include "cli_application.hpp"
#endif
namespace general {
namespace server {
namespace test {

TEST (TestApplication, generalApplicationSIGINT) {
#ifdef GRAPHICS
    using ApplicationType = GUIApplication;
    const char* argv[] = {"aboba"};
    ApplicationType application(1, const_cast<char**>(argv));
#else
    using ApplicationType = CLIApplication;
    ApplicationType application;
#endif
    std::jthread th {[] {
        constexpr std::chrono::milliseconds timeout {500};
        std::cerr << "Allotted " << timeout << "ms to initialize an app. "
            "If failed because of SIGINT, application couldn't setup for that time\n";
        std::this_thread::sleep_for(timeout);
        std::raise(SIGINT);
    }};
    application.run();
}

}
}
}
