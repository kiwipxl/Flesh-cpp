#ifndef _UTILITY_DEFINES_H_
#define _UTILITY_DEFINES_H_

#include <functional>

#define BEGIN_UTILITY_NS    namespace utility {
#define END_UTILITY_NS      };

//forward declares begin
BEGIN_UTILITY_NS
    
    typedef std::function<void()> InvokerFn;
    
END_UTILITY_NS
//forward declares end

#endif