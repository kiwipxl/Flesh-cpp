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
    extern CFTYPE FT_RESPONSE;

    //================== MID begin ==================

    struct Message {

        //identifier
        MID* mid;

        //params
        std::vector<CFTYPE> types;
        std::vector<MessageParam*> params;
        int total_bytes;
        int count;
    };

	struct MID {

		const int id = 0;
        const char* name;

		MID(int num_args, ...);
    };

	extern std::vector<msg::MID*> MID_list;
    extern std::vector<std::string> MID_names;

    //================== All MID constants ==================

    enum MID_enum {

        #define MID_DEF(x, y) x = y, 
        #include "test.def"
        #undef MID_DEF
    };
};

#endif