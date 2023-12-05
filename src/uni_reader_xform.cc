#include "uni_reader_xform.h"
#include <pxr/usd/usdGeom/xform.h>
#include "uni_prerequisites.h"
#include "utils/uni_math_matrix.h"

namespace universe 
{


bool UniXformReader::PrimHasXformOps() const
{
    pxr::UsdGeomXformable xformable(prim_);

    if(!xformable) {
        return false;
    }

    bool reset_xform_stack = false;
    return !xformable.GetOrderedXformOps(&reset_xform_stack).empty();
}

void UniXformReader::CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime, sd::UniScene *scene)
{
    node_ = uni_sd->CreateNode(name_, scene);
}

void UniXformReader::ReadNodeData(sd::UniSceneDescription *uni_sd, double motionSampleTime)
{
    bool is_constant;
    float transform_from_usd[4][4];

    ReadMatrix(transform_from_usd, motionSampleTime, settings_.scale, &is_constant);

    //useless?
    if(!is_constant) {
        
    }

    //apply 
    node_->has_matrix = true;
    memcpy(node_->matrix, transform_from_usd, sizeof(UniFloat) * 16);
}

void UniXformReader::ReadMatrix(float r_mat[4][4], float time, float scale, bool *r_is_constant)
{
    assert(r_mat);
    assert(r_is_constant);

    *r_is_constant = true;
    
    util::unit_m4(r_mat);

    std::optional<XformResult> xf_result = GetLocalUsdXform(time);
    if(!xf_result)
    {
        return;
    }

    std::get<0>(*xf_result).Get(r_mat);
    *r_is_constant = std::get<1>(*xf_result);

    /* Apply global scaling and rotation only to root objects, parenting will propagate it.*/
    if((scale != 1.0 || settings_.do_convert_mat) && is_root_xform_) {
        if(scale != 1.0f) {

        }
    }

}

bool UniXformReader::IsRootXformPrim() const {
    if(!prim_.IsValid()) {
        return false;
    }

    if(prim_.IsInPrototype()) {
        /* We don't consider prototypes to be root prims, 
        * because we never want to apply global scaling or
        * rotations to the prototypes themselves*/
        std::cout << "The prim " << prim_.GetName().GetString() << " is in prototype!" << std::endl;
        return false;
    }

    if(prim_.IsA<pxr::UsdGeomXformable>()) {
        /* If this prim doesn't have an ancestor that's a
        * UsdGeomXformable, then it's a root prim.  Note
        * that it's not sufficient to only check the immediate
        * parent prim, since the immediate parent could be a
        * UsdGeomScope that has an xformable ancestor. */
        pxr::UsdPrim cur_parent = prim_.GetParent();

        if(use_parent_xform_) {
            cur_parent = cur_parent.GetParent();
        }

        while(cur_parent && !cur_parent.IsPseudoRoot()) {
            if(cur_parent.IsA<pxr::UsdGeomXformable>()) {
                return false;
            }
            cur_parent = cur_parent.GetParent();
        }

        return true;
    }

    return false;
}

std::optional<XformResult> UniXformReader::GetLocalUsdXform(float time) const
{
    pxr::UsdGeomXformable xformable = use_parent_xform_ ? pxr::UsdGeomXformable(prim_.GetParent()) :
                                                            pxr::UsdGeomXformable(prim_);
    
    if(!xformable)
    {
        /* This might happen if the prim is a Scope. */
        return std::nullopt;
    }

    bool is_constant = !xformable.TransformMightBeTimeVarying();
    
    bool reset_xform_stack;
    pxr::GfMatrix4d xform;
    if(!xformable.GetLocalTransformation(&xform, &reset_xform_stack, time)){
        return std::nullopt;
    }

    return XformResult(pxr::GfMatrix4f(xform), is_constant);
}

}

