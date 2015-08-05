#include "MID.h"

#include <sstream>

using msg::MID;
using msg::FormatType;

FormatType::FormatType(const char* c, const short l, const char* name) : printf_format(c), len(l), type_name(name) { }

//format types for packing and unpacking byte data
CFTYPE msg::FT_CHAR								= new FormatType("%c", 1, "char");
CFTYPE msg::FT_SIGNED_CHAR						= new FormatType("%c", 1, "schar");
CFTYPE msg::FT_UNSIGNED_CHAR					= new FormatType("%c", 1, "uchar");
CFTYPE msg::FT_BOOL								= new FormatType("%d", 1, "bool");
CFTYPE msg::FT_SHORT							= new FormatType("%d", 2, "short");
CFTYPE msg::FT_UNSIGNED_SHORT					= new FormatType("%d", 2, "ushort");
CFTYPE msg::FT_INT								= new FormatType("%i", 4, "int");
CFTYPE msg::FT_UNSIGNED_INT						= new FormatType("%u", 4, "uint");
CFTYPE msg::FT_LONG								= new FormatType("%li", 8, "long");
CFTYPE msg::FT_UNSIGNED_LONG					= new FormatType("%lu", 8, "ulong");
CFTYPE msg::FT_LONG_LONG						= new FormatType("%lli", 8, "llong");
CFTYPE msg::FT_UNSIGNED_LONG_LONG				= new FormatType("%llu", 8, "ullong");
CFTYPE msg::FT_FLOAT							= new FormatType("%f", 4, "float");
CFTYPE msg::FT_DOUBLE							= new FormatType("%f", 8, "double");
CFTYPE msg::FT_CHAR_ARRAY						= new FormatType("%s", 1, "char*");
CFTYPE msg::FT_VOID_POINTER						= new FormatType("%p", 4, "void*");
CFTYPE msg::FT_RESPONSE                         = new FormatType("%d", 2, "response code");

int MID_id = 0;
std::vector<CMID> msg::MID_list;
std::ostringstream str_stream;

inline const char* get_name(msg::MID_enum e) {
    int count = 0;
    #define MID_DEF(x) if ((int)e == count) return #x; ++count;
    #include "MID_Defines.def"
    #undef MID_DEF

    return "MID_NAME_NOT_FOUND";
}

MID::MID() {
    id = MID_id;
    ++MID_id;
}

void msg::MID_init() {
    int count = 0;
    #define MID_DEF(x) ++count;
    #include "MID_Defines.def"
    #undef MID_DEF

    for (int n = 0; n < count; ++n) {
        MID* mid = new MID();
        mid->name = get_name((MID_enum)n);
        MID_list.push_back(mid);
    }
}