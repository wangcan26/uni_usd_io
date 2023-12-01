#include "uni_reader_xform.h"
#include <pxr/usd/usdGeom/xform.h>
#include <iostream>
#include <cassert>

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

void UniXformReader::CreateNode(UniSceneDescription *uni_sd, double motionSampleTime, UniScene *scene)
{
    node_ = uni_sd->CreateNode(name_, scene);
}

void UniXformReader::ReadNodeData(UniSceneDescription *uni_sd, double motionSampleTime)
{
    bool is_constant;
    float transform_from_usd[4][4];
}

void UniXformReader::ReadMatrix(float r_mat[4][4], float time, float scale, bool *r_is_constant)
{
    assert(r_mat);
    assert(r_is_constant);

    *r_is_constant = true;

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

}

