#pragma once

namespace p2ptransfer {

class Logger {
public:
    static std::osyncstream log() {
#ifdef TESTING
        return std::osyncstream(std::cerr);
#else
        return std::osyncstream(std::cout);
#endif
    }
    static void log_stacktrace(const std::string& err_header) {
#ifndef NDEBUG
        log() 
            << ERROR_COLOR << err_header << CLEAR_COLOR << std::endl
            << boost::stacktrace::stacktrace() << std::endl;
#ifdef TESTING
        std::terminate();
#endif
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
