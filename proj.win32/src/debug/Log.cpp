#include "Log.h"

extern void console_log(const char * format, ...) { CC_FORMAT_PRINTF(1, 2); }
