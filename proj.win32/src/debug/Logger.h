#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sstream>
#include "../SceneManager.h"

#define file_log debug::Logger(debug::ACTION_SAVE_TO_FILE)
#define print_log debug::Logger(debug::ACTION_PRINT)
#define file_print_log debug::Logger(debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)

namespace debug {

    enum LogAction {

        ACTION_SAVE_TO_FILE = 1,
        ACTION_PRINT = 2
    };

    class Logger {

        public:
            Logger(LogAction laction) {
                action = laction;
            }

            Logger(int laction) {
                action = (LogAction)laction;
            }

            std::stringstream stream;
            LogAction action;

            template<class T> Logger& operator<<(const T& v) {
                stream << v;
                return *this;
            }

            ~Logger() {
                if (action & ACTION_SAVE_TO_FILE) {
                    int a = 5;
                }else if (action & ACTION_PRINT) {
                    cc::log(stream.str().c_str());
                }
            }
    };

    extern void init_logger();
};

#endif