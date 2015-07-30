#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sstream>

#define log debug::Logger()

enum LogAction {

    ACTION_SAVE_TO_FILE,
    ACTION_PRINT
};

namespace debug {

    class Logger {

        std::stringstream stream;

        template<class T> Logger& operator<<(T& v) {
            if (v == SAVE_TO_FILE) {
            }else if (v == ACTION_PRINT) {
            }else {
                stream << v;
            }
        }
    };

    extern void init_logger();
};

#endif