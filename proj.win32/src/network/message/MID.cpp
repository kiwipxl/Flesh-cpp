#include "network/message/MID.h"

#include <cassert>
#include <sstream>

using msg::MID;
using msg::FormatType;

std::vector<CFTYPE> msg::FT_list;

FormatType::FormatType(const char* c, const short l, const char* name) : printf_format(c), len(l), type_name(name) { FT_list.push_back(this); }

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
CFTYPE msg::FT_STRING						    = new FormatType("%s", 1, "string");
CFTYPE msg::FT_VOID_POINTER						= new FormatType("%p", 4, "void*");

int MID_id = 0;
std::vector<CMID> msg::MID_list;
std::ostringstream str_stream;

inline const char* get_name(msg::MID_enum e) {
    int count = 0;
    #define MID_DEF(x) if ((int)e == count) return #x; ++count;
    #define ARG(x)
    #include "MID_Defines.def"
    #undef MID_DEF
    #undef ARG

    return "MID_NAME_NOT_FOUND";
}

MID::MID() {
    id = MID_id;
    ++MID_id;
}

MID* last_MID = NULL;
void create_MID(int index) {
    MID* mid = new MID();
    mid->name = get_name((msg::MID_enum)index);
    msg::MID_list.push_back(mid);
    last_MID = mid;
}

void add_arg(char* str) {
    bool found = false;
    for (int n = 0; n < msg::FT_list.size(); ++n) {
        if (strcmp(msg::FT_list[n]->type_name, str) == 0) {
            found = true;
            if (last_MID) {
                last_MID->param_types.push_back(msg::FT_list[n]);
                ++last_MID->param_count;
                last_MID->param_total_bytes += msg::FT_list[n]->len;
            }else {
                assert(!"argument used before the first MID define");
            }
        }
    }
    if (!found) assert(!"argument name could not be found in FT_list");
}

void msg::MID_init() {
    int count = 0;
    #define MID_DEF(x) create_MID(count); ++count;
    #define ARG(x) add_arg(#x);
    #include "MID_Defines.def"
    #undef MID_DEF
    #undef ARG
}

CMID msg::get_MID(MID_enum mid_enum) {
    return (mid_enum > 0 && mid_enum < MID_list.size()) ? MID_list[mid_enum] : MID_list[0];
}