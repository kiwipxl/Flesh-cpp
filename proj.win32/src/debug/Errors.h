#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "PlatformConfig.h"

namespace err {

    #define NO_ERROR 0
    #define UDP_PING_PONG_TIMEOUT 9000

    extern int get_last_error();
    extern int fresult;
};

#endif