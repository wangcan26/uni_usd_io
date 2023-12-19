#pragma once
#include "uni_reader_prim.h"
#include <pxr/base/gf/matrix4f.h>

namespace universe 
{
using XformResult = std::tuple<pxr::GfMatrix4f, bool>;
class UniXformReader : public UniPrimReader {
public:
    UniXformReader(const pxr::UsdPrim &prim, const UniSettings& settings)
        : UniPrimReader(prim, settings),
        use_parent_xform_(false),
        is_root_xform_(IsRootXformPrim())
    {

    }

    void CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime, sd::UniScene *scene = nullptr) override;
    void ReadNodeData(sd::UniSceneDescription *uni_sd, double motionSampleTime) override;

    void ReadMatrix(float r_mat[4][4], float time, float scale, bool *r_is_constant);

    bool PrimHasXformOps() const;

    bool UseParentXform() const {
        return use_parent_xform_;
    }

    void SetUsdParentXform(bool flag)
    {
        use_parent_xform_ = flag;
        is_root_xform_ = IsRootXformPrim();
    }

protected:
    /* Returns true if the contained USD pprim is the root of a transform hierarchy.*/
    bool IsRootXformPrim() const;

    // Return the USD prim's local transformation.
    // param time: Time code for evaluating the transform.
    // return value: Optional tuple with the following elements:
    //               - The transform matrix.
    //               - A boolean flag indicating whether the matrix is constant over time
    virtual std::optional<XformResult> GetLocalUsdXform(float time) const;

private:
    bool use_parent_xform_;
    bool is_root_xform_;


};

}