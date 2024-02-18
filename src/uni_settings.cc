#include "uni_settings.h"
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <pxr/usd/usd/prim.h>

namespace universe
{

namespace import 
{

namespace util
{

UNI_EXPORT void UNI_EXPORTED_FUNC convert_to_z_up(pxr::UsdStageRefPtr stage, UniSettings *settings)
{
    bool isZUp = false;
    if(pxr::UsdGeomGetStageUpAxis(stage) == pxr::UsdGeomTokens->z)
    {
        isZUp = true;
    }

    if(isZUp)
    {
        settings->do_convert_mat = true;
        settings->up_z = true;
        std::cout << "Usd Scene is z-up!" << std::endl;
        return;
    }
}

UNI_EXPORT void UNI_EXPORTED_FUNC find_prefix_to_skip(pxr::UsdStageRefPtr stage, UniSettings *settings)
{
    if(!stage) {
        return;
    }

    pxr::TfToken generated_key("Blender:generated");
    pxr::SdfPath path("/");
    auto prim = stage->GetPseudoRoot();
    while(true) {
        uint32_t child_count = 0;
        for(auto child : prim.GetChildren())
        {
            if(child_count == 0) {
                prim = child.GetPrim();
            }
            ++child_count;
        }

        if(child_count != 1){
            break;
        }

        if(!prim.HasCustomDataKey(generated_key)) {
            break;
        }

        path = path.AppendChild(prim.GetName());
    }

    auto path_string = path.GetString();
    if(path == pxr::SdfPath("/")) {
        path = pxr::SdfPath();
    }

    settings->skip_prefix = path;
}

} //util

} //usd

}