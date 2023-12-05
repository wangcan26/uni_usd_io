#pragma once 

#include "uni_reader_geom.h"
#include "pxr/usd/usdGeom/mesh.h"

namespace universe 
{

class UniMeshReader : public UniGeomReader {
public:
    UniMeshReader(const pxr::UsdPrim &prim, const UniSettings& settings);

    void CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime, sd::UniScene *scene = nullptr) override;
    void ReadNodeData(sd::UniSceneDescription *uni_sd, double notionSampleTime) override;

private:
    pxr::UsdGeomMesh mesh_prim_;
    bool is_left_handed_;
};

}