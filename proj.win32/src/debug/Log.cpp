#include "Log.h"

extern void console_log(const char* format, ...) { CCLOG(format, __VA_ARGS__); }
