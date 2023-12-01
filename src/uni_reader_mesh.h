#pragma once 

#include "uni_reader_geom.h"
#include "pxr/usd/usdGeom/mesh.h"

namespace universe 
{

class UniMeshReader : public UniGeomReader {
public:
    UniMeshReader(const pxr::UsdPrim &prim, const UniSettings& settings);

    void CreateNode(UniSceneDescription *uni_sd, double motionSampleTime, UniScene *scene = nullptr) override;
    void ReadNodeData(UniSceneDescription *uni_sd, double notionSampleTime) override;
};

}