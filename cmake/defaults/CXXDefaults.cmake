include(CXXHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

if(MSVC)
    include(msvcdefaults)
endif()

_add_define(GL_GLEXT_PROTOTYPES)
_add_define(GLX_GLXEXT_PROTOTYPES)

# Python bindings for tf require this define.
_add_define(BOOST_PYTHON_NO_PY_SIGNATURES)

# Parts of boost (in particular, boost::hash) rely on deprecated features
# of the STL that have been removed from some implementations under C++17.
# This define tells boost not to use those features.
#
# Under Visual Studio, boost automatically detects that this flag is
# needed so we don't need to define it ourselves. Otherwise, we'll get a
# C4005 macro redefinition warning.
if (NOT MSVC)
    _add_define(BOOST_NO_CXX98_FUNCTION_BASE)
endif()

# CMake list to string.
string(REPLACE ";" " "  _UNI_CXX_FLAGS "${_UNI_CXX_FLAGS}")