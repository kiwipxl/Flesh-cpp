#ifndef _ERRORS_H_
#define _ERRORS_H_

#include "PlatformConfig.h"

namespace err {

    #define NO_ERROR 0

    extern int get_last_error();
    extern int fresult;
};

#endif