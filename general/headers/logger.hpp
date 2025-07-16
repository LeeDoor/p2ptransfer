#pragma once
namespace general {

class Logger {
public:
    static std::osyncstream log() {
        return std::osyncstream(std::cout);
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
};

}
