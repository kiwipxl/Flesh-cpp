#include "network/message/MID.h"

#include <cassert>
#include <sstream>

BEGIN_NETWORK_NS

namespace msg {

    std::vector<CFTYPE> FT_list;

    FormatType::FormatType(const char* c, const short l, const char* name) : printf_format(c), len(l), type_name(name) { FT_list.push_back(this); }

    //format types for packing and unpacking byte data
    CFTYPE FT_CHAR								= new FormatType("%c", 1, "char");
    CFTYPE FT_SIGNED_CHAR						= new FormatType("%c", 1, "schar");
    CFTYPE FT_UNSIGNED_CHAR					= new FormatType("%c", 1, "uchar");
    CFTYPE FT_BOOL								= new FormatType("%d", 1, "bool");
    CFTYPE FT_SHORT							= new FormatType("%d", 2, "short");
    CFTYPE FT_UNSIGNED_SHORT					= new FormatType("%d", 2, "ushort");
    CFTYPE FT_INT								= new FormatType("%i", 4, "int");
    CFTYPE FT_UNSIGNED_INT						= new FormatType("%u", 4, "uint");
    CFTYPE FT_LONG								= new FormatType("%li", 8, "long");
    CFTYPE FT_UNSIGNED_LONG					= new FormatType("%lu", 8, "ulong");
    CFTYPE FT_LONG_LONG						= new FormatType("%lli", 8, "llong");
    CFTYPE FT_UNSIGNED_LONG_LONG				= new FormatType("%llu", 8, "ullong");
    CFTYPE FT_FLOAT							= new FormatType("%f", 4, "float");
    CFTYPE FT_DOUBLE							= new FormatType("%f", 8, "double");
    CFTYPE FT_STRING						    = new FormatType("%s", 1, "string");
    CFTYPE FT_VOID_POINTER						= new FormatType("%p", 4, "void*");

    int MID_id = 0;
    std::vector<CMID> MID_list;
    std::ostringstream str_stream;

    inline const char* get_name(MID_enum e) {
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
        mid->name = get_name((MID_enum)index);
        MID_list.push_back(mid);
        last_MID = mid;
    }

    void add_arg(char* str) {
        bool found = false;
        for (int n = 0; n < FT_list.size(); ++n) {
            if (strcmp(FT_list[n]->type_name, str) == 0) {
                found = true;
                if (last_MID) {
                    last_MID->param_types.push_back(FT_list[n]);
                    ++last_MID->param_count;
                    last_MID->param_total_bytes += FT_list[n]->len;
                }else {
                    assert(!"argument used before the first MID define");
                }
            }
        }
        if (!found) assert(!"argument name could not be found in FT_list");
    }

    void MID_init() {
        int count = 0;
        #define MID_DEF(x) create_MID(count); ++count;
        #define ARG(x) add_arg(#x);
        #include "MID_Defines.def"
        #undef MID_DEF
        #undef ARG
    }

    CMID get_MID(MID_enum mid_enum) {
        return (mid_enum > 0 && mid_enum < MID_list.size()) ? MID_list[mid_enum] : MID_list[0];
    }
};

END_NETWORK_NS
