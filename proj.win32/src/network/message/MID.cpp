#include "MID.h"

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

int msg::MID_id = 0;
std::vector<CMID> msg::MID_list;
std::vector<std::string> msg::MID_names;

MID::MID(int num_args, ...) : id(MID_id) {
    if (num_args > 0) ft_params = new CFTYPE[num_args];
    num_params = num_args;

    va_list ap;
    va_start(ap, num_args);
    for (int n = 0; n < num_args; ++n) {
        CFTYPE ft = va_arg(ap, CFTYPE);
        ft_params[n] = ft;
        total_param_bytes += ft->len;
    }
    va_end(ap);

    ++MID_id;
    MID_list.push_back(this);
}

msg::MIDConstants* msg::MIDConstants::mid_constant;