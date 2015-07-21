#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "PlatformConfig.h"

#define ERR_NONE 0
#define ERR_UDP_PING_PONG_FAILED 9000

namespace err {

    extern int get_last_error();
    extern int fresult;
};

#endif