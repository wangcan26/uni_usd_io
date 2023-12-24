#pragma once 

#include <vector>
#include <iostream>
#include <cassert>
#include <stddef.h>
#include <string>
#include <optional>

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
