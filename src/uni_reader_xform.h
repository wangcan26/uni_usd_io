#pragma once
#include "uni_reader_prim.h"

namespace universe 
{

class UniXformReader : public UniPrimReader {
public:
    UniXformReader(const pxr::UsdPrim &prim, const UniSettings& settings)
        : UniPrimReader(prim, settings),
        use_parent_xform_(false)
    {

    }

    void CreateNode(UniSceneDescription *uni_sd, double motionSampleTime, UniScene *scene = nullptr) override;
    void ReadNodeData(UniSceneDescription *uni_sd, double motionSampleTime) override;

    void ReadMatrix(float r_mat[4][4], float time, float scale, bool *r_is_constant);

    bool PrimHasXformOps() const;

    bool UseParentXform() const {
        return use_parent_xform_;
    }

    void SetUsdParentXform(bool flag)
    {
        use_parent_xform_ = flag;
    }

protected:
    /* Returns true if the contained USD pprim is the root of a transform hierarchy.*/
    bool IsRootXformPrim() const;

private:
    bool use_parent_xform_;


};

}