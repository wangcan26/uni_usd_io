#pragma once

#include <pxr/usd/usd/stage.h>
#include "uni_scene_description.h"
#include "uni_reader_prim.h"
#include "uni_settings.h"
#include "uni_prerequisites.h"

namespace universe 
{

class UniStageReader {
public:
    UniStageReader(pxr::UsdStageRefPtr stage, const UniSettings &settings);
    ~UniStageReader();

    UniPrimReader *CreateReaderIfAllowed(const pxr::UsdPrim &prim);

    void CollectReaders(sd::UniSceneDescription *sd);

    bool Valid() const;

    void ClearReaders();

    sd::UniScene *CurrentScene() const;

    const std::vector<UniPrimReader*> &Readers() const
    {
        return readers_;
    }

private:
    UniPrimReader *CollectReaders(sd::UniSceneDescription* sd, const pxr::UsdPrim &prim);

protected:
    pxr::UsdStageRefPtr stage_;
    std::vector<UniPrimReader *> readers_;
    sd::UniScene *cur_scene_;
    UniSettings settings_;
};


}