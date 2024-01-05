#pragma once 

#include "uni_reader_geom.h"
#include "pxr/usd/usdGeom/mesh.h"

namespace universe 
{

class UniMeshReader : public UniGeomReader {
public:
    UniMeshReader(const pxr::UsdPrim &prim, const UniSettings& settings);

    void CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime) override;
    void ReadNodeData(sd::UniSceneDescription *uni_sd, double motionSampleTime) override;

private:
    pxr::UsdGeomMesh mesh_prim_;
    bool is_left_handed_;
    pxr::VtIntArray     face_inidices_;
    pxr::VtIntArray     face_counts_;
    pxr::VtVec3fArray   positions_;
};

}