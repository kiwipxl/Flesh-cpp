from __future__ import print_function;
import sys;

class FormatType():

    struct_char = '';
    printf_char = '';
    len = 0;
    type_name = "";

    def __init__(self, schar, pchar, len, name):
        self.struct_char = schar;
        self.printf_char = pchar;
        self.len = len;
        self.type_name = name;

#format types for packing and unpacking byte data
CHAR                    = FormatType('c', 'c', 1, "char");
SIGNED_CHAR             = FormatType('b', 'c', 1, "schar");
UNSIGNED_CHAR           = FormatType('B', 'c', 1, "uchar");
BOOL                    = FormatType('?', 'd', 1, "bool");
SHORT                   = FormatType('h', 'd', 2, "short");
UNSIGNED_SHORT          = FormatType('H', 'd', 2, "ushort");
INT                     = FormatType('i', 'i', 4, "int");
UNSIGNED_INT            = FormatType('I', 'u', 4, "uint");
LONG                    = FormatType('l', 'li', 8, "long");
UNSIGNED_LONG           = FormatType('L', 'lu', 8, "ulong");
LONG_LONG               = FormatType('q', 'lli', 8, "llong");
UNSIGNED_LONG_LONG      = FormatType('Q', 'llu', 8, "ullong");
FLOAT                   = FormatType('f', 'f', 4, "float");
DOUBLE                  = FormatType('d', 'f', 8, "double");
STRING                  = FormatType('s', 's', 1, "string");
VOID_POINTER            = FormatType('p', 'lu', 4, "void*");
GENERAL_RESULT          = FormatType('H', 'd', 2, "gresult");
