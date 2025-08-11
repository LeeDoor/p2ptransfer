#include "cli_application.hpp"
namespace general {
namespace server {
namespace test {

TEST (TestApplication, generalApplicationSIGINT) {
    using namespace std::chrono_literals;
    std::chrono::milliseconds timeout = 0ms;
    using ApplicationType = CLIApplication;
    ApplicationType application;
    timeout = 100ms;
    std::jthread th {[&timeout] {
        std::cerr << "Allotted " << timeout << " to initialize an app. "
            "If failed because of SIGINT, application couldn't setup for that time\n";
        std::this_thread::sleep_for(timeout);
        std::raise(SIGINT);
    }};
    application.run();
}

}
}
}
