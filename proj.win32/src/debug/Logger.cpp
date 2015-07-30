#include "Logger.h"

FILE* debug::lfile;

void debug::init_logger() {
    lfile = fopen("log.txt", "w");

    if (lfile != NULL) {

    }
}