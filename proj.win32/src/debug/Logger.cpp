#include "debug/Logger.h"

FILE* debug::lfile;

void debug::init_logger() {
    lfile = fopen("log.txt", "w");
    cf_assert(lfile == NULL, sstream << "log.txt could not be written");
}

//taken from mingw wassert.c
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
