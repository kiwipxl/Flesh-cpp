#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <sstream>
#include "../SceneManager.h"

#define log_file debug::Logger(debug::ACTION_INFO | debug::ACTION_SAVE_TO_FILE)
#define log_print debug::Logger(debug::ACTION_INFO | debug::ACTION_PRINT)
#define log_print_file debug::Logger(debug::ACTION_INFO | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)

#define log_info debug::Logger(debug::ACTION_INFO | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)
#define log_warning debug::Logger(debug::ACTION_WARNING | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)
#define log_error debug::Logger(debug::ACTION_WARNING | debug::ACTION_SAVE_TO_FILE | debug::ACTION_PRINT)

namespace debug {

    enum LogAction {

        ACTION_INFO = 1 << 1, 
        ACTION_WARNING = 1 << 2, 
        ACTION_ERROR = 1 << 3, 

        ACTION_SAVE_TO_FILE = 1 << 4,
        ACTION_PRINT = 1 << 5, 
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
                    if (action & ACTION_INFO) {
                        cc::log(stream.str().c_str());
                    }else if (action & ACTION_WARNING) {
                        cc::log(stream.str().c_str());
                    }else if (action & ACTION_ERROR) {
                        cc::log(stream.str().c_str());
                    }
                }
            }
    };

    extern void init_logger();
};

#endif