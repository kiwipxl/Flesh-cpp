#ifndef _READABLE_TYPE_H_
#define _READABLE_TYPE_H_

/*
readable types code sample found here http://ideone.com/GZEGN6
*/

#include <iostream>
#include <string>
#include <typeinfo>

#ifdef __GNUG__ // GCC

#include <cxxabi.h>
#include <cstdlib>

static std::string readable_name(const char* mangled_name)
{
	int status;
	char* temp = __cxxabiv1::__cxa_demangle(mangled_name, nullptr, nullptr, &status);
	if (temp)
	{
		std::string result(temp);
		std::free(temp);
		return result;
	}
	else return mangled_name;
}

#else // not GCC

inline std::string readable_name(const char* mangled_name) { return mangled_name; }

#endif // __GNUG__

template < typename T > std::string type_to_string()
{
	return readable_name(typeid(T).name());
}

template < typename T > std::string type_to_string(const T& obj)
{
	return readable_name(typeid(obj).name());
}

#endif
