#include "uni_reader_mesh.h"

namespace universe {

UniMeshReader::UniMeshReader(const pxr::UsdPrim &prim, const UniSettings& settings)
    : UniGeomReader(prim, settings)
{

}

void UniMeshReader::CreateNode(UniSceneDescription *uni_sd, double motionSampleTime, UniScene *scene)
{
    node_ = uni_sd->CreateNode(name_, nullptr);
}

void UniMeshReader::ReadNodeData(UniSceneDescription *uni_sd, double notionSampleTime)
{
    
}

}