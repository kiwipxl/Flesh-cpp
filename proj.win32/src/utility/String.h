#ifndef _STRING_H_
#define _STRING_H_

#include <functional>
#include <sstream>

#include "utility/Defines.h"

BEGIN_UTILITY_NS

#define sstream_str(x)      (utility::glb_stream.clear() << x << '\0').stream.str()
#define sstream_cstr(x)     (utility::glb_stream.clear() << x << '\0').stream.str().c_str()

class StrStream {

public:
    std::stringstream stream;

    StrStream& clear() {
        stream.str("");
        return *this;
    }

    template<class T> StrStream& operator<<(const T& v) {
        stream << v;
        return *this;
    }
};

extern StrStream glb_stream;

END_UTILITY_NS

#endif