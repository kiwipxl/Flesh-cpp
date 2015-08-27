#ifndef _GENERAL_H_
#define _GENERAL_H_

#include "utility/Defines.h"

BEGIN_UTILITY_NS

void invoke_main_thread(InvokerFn fn);

void update_general();

END_UTILITY_NS

#endif