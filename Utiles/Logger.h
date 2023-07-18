#pragma once
#include <iostream>
#include <fstream>


class Logger {
public:
    static Logger& get() {
        if (!logger) {
            logger.reset(new Logger);
            logger->clear();
        }

        return *logger;
    }
    
    void println(const std::string& message, bool is_error = false) {
        print(message + '\n', is_error);
    }
    void print(const std::string& message, bool is_error = false) {
        std::ofstream flog(path, std::ios_base::app);
        flog << message << '\n';
        if (flog.fail()) {
            std::cerr << "Can't write to " + path << "\nMessege was:\n" + message;
            is_error = true;
        }
        flog.close();
        if (is_error) {
            exit(0);
        }
    }
    
    void clear() {
        std::ofstream flog(path, std::ios_base::trunc);
        if (!flog.is_open()) {
            std::cerr << "Can't write to " + path;
            exit(0);
        }
        flog.close();
    }

private:
    Logger() = default;

private:
    static std::unique_ptr<Logger> logger;
public:
    static std::string path;
};

