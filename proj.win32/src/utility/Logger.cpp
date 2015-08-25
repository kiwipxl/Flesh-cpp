#include "utility/Logger.h"

#include <assert.h>

#include <2d/CCNode.h>

#include "utility/General.h"
#include "utility/String.h"

BEGIN_UTILITY_NS

//private
std::FILE* lfile;

//public

void init_logger() {
    lfile = fopen("log.txt", "w");
    cf_assert(lfile == NULL, sstream_cstr("log.txt could not be written"));
}

//-- begin Logger class --

Logger::~Logger() {
    if (!(action & ACTION_NO_NEW_LINE)) stream << '\n';

    if (action & ACTION_SAVE_TO_FILE) {
        fwrite(stream.str().c_str(), stream.str().length(), 1, lfile);
        fflush(lfile);
    }
    if (action & ACTION_PRINT) {
        if (action & ACTION_INFO) {
            cocos2d::log(stream.str().c_str());
        }else if (action & ACTION_WARNING) {
            cocos2d::log(stream.str().c_str());
        }else if (action & ACTION_ERROR) {
            cocos2d::log(stream.str().c_str());
        }else if (action & ACTION_MID) {
            cocos2d::log(stream.str().c_str());
        }
    }
}

//-- end Logger class --

END_UTILITY_NS

//private

//taken from mingw wassert.c
//converts assert into wchar assert
void __cdecl _assert(const char *_Message, const char *_File, unsigned _Line) {
    wchar_t *m, *f;
    int i;
    m = (wchar_t *)malloc((strlen(_Message) + 1) * sizeof(wchar_t));
    f = (wchar_t *)malloc((strlen(_File) + 1) * sizeof(wchar_t));
    for (i = 0; _Message[i] != 0; i++)
        m[i] = ((wchar_t)_Message[i]) & 0xff;
    m[i] = 0;
    for (i = 0; _File[i] != 0; i++)
        f[i] = ((wchar_t)_File[i]) & 0xff;
    f[i] = 0;
    _wassert(m, f, _Line);
    free(m);
    free(f);
}

void f_assert(const char* message) {
    #ifndef NDEBUG
        _assert(message, __FILE__, __LINE__);
    #endif
    log_error << "assert occurred in " << __FILE__ << " line " << __LINE__ << ": " << sstream_cstr((message));
}

void cf_assert(bool condition, const char* message) {
    if (!condition) return;

    #ifndef NDEBUG
        _assert(message, __FILE__, __LINE__);
    #endif
    log_error << "assert occurred in " << __FILE__ << " line " << __LINE__ << ": " << sstream_cstr((message));
}