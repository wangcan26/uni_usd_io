#pragma once 

#include <vector>
#include <iostream>
#include <cassert>
#include <stddef.h>
#include <string>
#include <optional>
#include <unordered_map>

#if defined(__MINGW32__)
    #define _EXPORT __declspec(dllexport)
#elif defined(_WIN32) || defined(__unix__) || defined(__APPLE__) // Use .def files for these
    #define _EXPORT
#else
    #error "Unknown platform/compiler"
#endif

#if _MSC_VER
    #define UNI_EXPORTED_FUNC   __cdecl
    #define UNI_EXPORTED_METHOD __thiscall
#else
    #define UNI_EXPORTED_FUNC
    #define UNI_EXPORTED_METHOD
#endif

 #define UNI_EXPORT _EXPORT

namespace universe
{

typedef size_t UniSize;
typedef long long int UniSSize;
typedef float UniFloat;
typedef int UniInt;
typedef unsigned int UniUint;
typedef int  UniBool;
typedef char UniByte;
#if UNI_PLATFORM_WINDOWS
    typedef unsigned short UniUShort;
#else
    typedef ushort UniUShort;
#endif

}
