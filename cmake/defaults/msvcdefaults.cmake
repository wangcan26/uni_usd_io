#Enable execption handling
set(UNI_CXX_FLAGS "${UNI_CXX_FLAGS} /EHsc")

# Standards compliant.
set(UNI_CXX_FLAGS "${UNI_CXX_FLAGS} /Zc:rvalueCast /Zc:strictStrings")

if(MSVC_VERSION GREATER_EQUAL 1920)
    set(UNI_CXX_FLAGS "${UNI_CXX_FLAGS} /Zc:inline-")
else()
    set(UNI_CXX_FLAGS "${UNI_CXX_FLAGS} /Zc:inline")
endif()

# Turn on all but informational warnings.
set(UNI_CXX_FLAGS "${UNI_CXX_FLAGS} /W3")

_disable_warning("4003")

# truncation from 'double' to 'float' due to matrix and vector classes in `Gf`
_disable_warning("4244")
_disable_warning("4305")

# conversion from size_t to int. While we don't want this enabled
# it's in the Python headers. So all the Python wrap code is affected.
_disable_warning("4267")

# no definition for inline function
# this affects Glf only
_disable_warning("4506")

# c:\python27\include\pymath.h(22): warning C4273: 'round': inconsistent dll linkage 
# XXX:figure out real fix
_disable_warning("4273")

# qualifier applied to function type has no meaning; ignored
# tbb/parallel_for_each.h
_disable_warning("4180")

# '<<': result of 32-bit shift implicitly converted to 64 bits
# tbb/enumerable_thread_specific.h
_disable_warning("4334")

# Disable warning C4996 regarding fopen(), strcpy(), etc.
_add_define("_CRT_SECURE_NO_WARNINGS")

# Disable warning C4996 regarding unchecked iterators for std::transform,
# std::copy, std::equal, et al.
_add_define("_SCL_SECURE_NO_WARNINGS")

# Make sure WinDef.h does not define min and max macros which
# will conflict with std::min() and std::max().
_add_define("NOMINMAX")

# Needed to prevent YY files trying to include unistd.h
# (which doesn't exist on Windows)
_add_define("YY_NO_UNISTD_H")

_add_define("BOOST_ALL_DYN_LINK")
_add_define("BOOST_ALL_NO_LIB")

# Need half::_toFloat and half::_eLut.
_add_define("OPENEXR_DLL")

# Exclude headers from unnecessary Windows APIs to improve build
# times and avoid annoying conflicts with macros defined in those
# headers.
_add_define("WIN32_LEAN_AND_MEAN")

# These files require /bigobj compiler flag
#   Vt/arrayPyBuffer.cpp
#   Usd/crateFile.cpp
#   Usd/stage.cpp
# Until we can set the flag on a per file basis, we'll have to enable it
# for all translation units.
set(_UNI_CXX_FLAGS "${_UNI_CXX_FLAGS} /bigobj")

# Enable PDB generation.
set(_UNI_CXX_FLAGS "${_UNI_CXX_FLAGS} /Zi")

# Enable multiprocessor builds.
set(_UNI_CXX_FLAGS "${_UNI_CXX_FLAGS} /MP")
set(_UNI_CXX_FLAGS "${_UNI_CXX_FLAGS} /Gm-")

# Ignore LNK4221.  This happens when making an archive with a object file
# with no symbols in it.  We do this a lot because of a pattern of having
# a C++ source file for many header-only facilities, e.g. tf/bitUtils.cpp.
set(CMAKE_STATIC_LINKER_FLAGS "${CMAKE_STATIC_LINKER_FLAGS} /IGNORE:4221")