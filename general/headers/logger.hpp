#pragma once
// If you don't want to use std::cout for logging, write in main file: 
// #define IS_COUT 0 
#ifndef IS_COUT
#define IS_COUT 1 
#else
#undef IS_COUT
#define IS_COUT 0
#endif
class Logger {
public:
    static void init() {
        initialized = true;
        if constexpr(is_cout_) {
            os_ = &std::cout;
        } else {
            std::ofstream* ofs = new std::ofstream;
            os_ = static_cast<std::ostream*>(ofs);
        }
    }
    static void destroy() {
        if constexpr(!is_cout_) {
            static_cast<std::ofstream*>(os_)->close();
            free(os_);
        }
    }
    static std::osyncstream log() {
        if(initialized) 
            return std::osyncstream(*os_);
        std::cerr << 
            "initialize the logger before usage: "
            "Logger::init(); and Logger::destroy() "
            "or create a LoggerInitializer variable "
            "at the beginning of your program." << std::endl;
        exit(1);
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
    private:
    static constexpr bool is_cout_ = IS_COUT;
    inline static std::ostream* os_;
    inline static bool initialized = false;
};
class LoggerInitializer {
public:
    LoggerInitializer() { Logger::init(); }
    ~LoggerInitializer() { Logger::destroy(); }
};

