#ifndef _MID_H_
#define _MID_H_

#include <vector>
#include <string>
#include <stdarg.h>

namespace msg {

    //================== Format types begin ==================

	struct FormatType {

		const char* printf_format;
		const short len;
		const char* type_name;

		FormatType(const char* c, const short l, const char* name);
	};

	#define CFTYPE const msg::FormatType*
    
    extern std::vector<CFTYPE> FT_list;

	//format types for packing and unpacking byte data
	extern CFTYPE FT_CHAR;
	extern CFTYPE FT_SIGNED_CHAR;
	extern CFTYPE FT_UNSIGNED_CHAR;
	extern CFTYPE FT_BOOL;
	extern CFTYPE FT_SHORT;
	extern CFTYPE FT_UNSIGNED_SHORT;
	extern CFTYPE FT_INT;
	extern CFTYPE FT_UNSIGNED_INT;
	extern CFTYPE FT_LONG;
	extern CFTYPE FT_UNSIGNED_LONG;
	extern CFTYPE FT_LONG_LONG;
	extern CFTYPE FT_UNSIGNED_LONG_LONG;
	extern CFTYPE FT_FLOAT;
	extern CFTYPE FT_DOUBLE;
	extern CFTYPE FT_CHAR_ARRAY;
    extern CFTYPE FT_VOID_POINTER;

    //================== MID begin ==================

    enum MID_enum {

        #define MID_DEF(x) x, 
        #define ARG(x) 
        #include "MID_Defines.def"
        #undef MID_DEF
        #undef ARG
    };

	struct MID {

        //attribs
        int id;
        const char* name;

        //params
        std::vector<CFTYPE> param_types;
        int param_total_bytes = 0;
        int param_count = 0;

		MID();
    };

    #define CMID const msg::MID const *

    extern std::vector<CMID> MID_list;

    extern void MID_init();
    extern CMID get_MID(MID_enum mid_enum);
};

#endif