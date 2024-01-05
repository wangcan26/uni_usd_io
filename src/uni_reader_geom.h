#pragma once

#include "uni_reader_xform.h"

namespace universe 
{

class UniGeomReader : public UniXformReader {
public:
    UniGeomReader(const pxr::UsdPrim& prim, const UniSettings& settings)
        : UniXformReader(prim, settings)
    {

    }
};

}