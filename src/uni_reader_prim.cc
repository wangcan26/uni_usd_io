#include "uni_reader_prim.h"

namespace universe
{

UniPrimReader::UniPrimReader(const pxr::UsdPrim& prim, const UniSettings& settings)
    :name_(prim.GetName().GetString()),
    prim_path_(prim.GetPrimPath().GetString()),
    prim_(prim),
    parent_(nullptr),
    node_(nullptr),
    settings_(settings)
{

}

UniPrimReader::~UniPrimReader()
{

}

const pxr::UsdPrim &UniPrimReader::Prim() const
{
    return prim_;
}

bool UniPrimReader::Valid() const 
{
    return prim_.IsValid();
}

sd::UniNode *UniPrimReader::Node() const
{
    return node_;
}

}