#include <iostream>
#include <pxr/usd/usd/stage.h>
#include <pxr/usd/usdGeom/metrics.h>
#include <pxr/usd/usdGeom/tokens.h>
#include <uni_scene_description.h>
#include <uni_reader_stage.h>
#include <memory>

int main() 
{
    std::cout << "Hello Universe IO " << std::endl;
    pxr::UsdStageRefPtr stage = pxr::UsdStage::Open("/Users/bytedance/Workspace/Filament/usdSamples/HelloUSD.usda");
    if(!stage) {
        std::cout << "Open USD stage failed!" << std::endl;
        return -1;
    }

    bool isZUp = false;
    if(pxr::UsdGeomGetStageUpAxis(stage) == pxr::UsdGeomTokens->z)
    {
        isZUp = true;
    }

    if(isZUp)
    {
        std::cout << "Usd Scene is z-up!" << std::endl;
    }

    universe::UniSettings uni_settings;
    uni_settings.scale = 1.0;
    std::unique_ptr<universe::UniSceneDescription> uni_sd = std::make_unique<universe::UniSceneDescription>();
    std::unique_ptr<universe::UniStageReader> archive = std::make_unique<universe::UniStageReader>(stage, uni_settings);
    archive->CollectReaders(uni_sd.get());

    //archive sort readers
    //...

    for(universe::UniPrimReader *reader : archive->Readers()){
        if(!reader){
            continue;
        }

        reader->CreateNode(uni_sd.get(), 0.0, archive->CurrentScene());
    }

    /* Setup parenthood and read actual object data */
    for(universe::UniPrimReader *reader : archive->Readers())
    {
        if(!reader){
            continue;
        }

        universe::UniNode *node = reader->Node();
        reader->ReadNodeData(uni_sd.get(), 0.0);
    }


    return 0;
}