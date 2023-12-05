#include "uni_reader_mesh.h"

namespace universe {

UniMeshReader::UniMeshReader(const pxr::UsdPrim &prim, const UniSettings& settings)
    : UniGeomReader(prim, settings),
    mesh_prim_(prim),
    is_left_handed_(false)
{

}

void UniMeshReader::CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime, sd::UniScene *scene)
{
    node_ = uni_sd->CreateNode(name_, nullptr);
}

void UniMeshReader::ReadNodeData(sd::UniSceneDescription *uni_sd, double notionSampleTime)
{
    
}  

}