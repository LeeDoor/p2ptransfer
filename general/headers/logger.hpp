#pragma once
namespace general {

class Logger {
public:
    static std::osyncstream log() {
#ifdef TESTING
        return std::osyncstream(std::cerr);
#else
        return std::osyncstream(std::cout);
#endif
    }
    static void progressbar_init() {
        log() << std::setprecision(5) << std::right;
    }
    static void progressbar_stop() {
        log() << std::left << std::endl;
    }
    static void log_progressbar(double persent) {
        log() << "\r" << std::flush  << 
            "progress: " << std::setw(10) << std::max(0.0, persent) << " %";
    }
    static constexpr std::string_view ERROR_COLOR = "\033[30;41m";
    static constexpr std::string_view CLEAR_COLOR = "\033[0;0m";
};

}
