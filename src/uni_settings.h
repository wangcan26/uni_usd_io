#pragma once
#include <pxr/usd/sdf/path.h>
#include <pxr/usd/usd/stage.h>
#include "uni_prerequisites.h"

namespace universe 
{

typedef struct UniSettings
{
    bool do_convert_mat;
    float scale;
    bool is_sequence;
    int  sequence_len;
    bool validate_meshes;
    
    pxr::SdfPath skip_prefix;

    UniSettings() :
        do_convert_mat(false),
        scale(1.0f),
        is_sequence(false),
        sequence_len(1),
        validate_meshes(false),
        skip_prefix(pxr::SdfPath{})
    {

    }
}UniSettings;

namespace import
{

namespace util
{

UNI_EXPORT void UNI_EXPORTED_FUNC convert_to_z_up(pxr::UsdStageRefPtr stage, UniSettings *settings);

UNI_EXPORT void UNI_EXPORTED_FUNC find_prefix_to_skip(pxr::UsdStageRefPtr stage, UniSettings *settings);


}

}

}