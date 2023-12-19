#include <iostream>
#include <pxr/usd/usd/stage.h>
#include <uni_scene_description.h>
#include <uni_reader_stage.h>
#include "uni_settings.h"
#include <memory>

int main() 
{
    std::cout << "Hello Universe IO " << std::endl;
    pxr::UsdStageRefPtr stage = pxr::UsdStage::Open("/Users/bytedance/Workspace/Filament/usdSamples/HelloUSD.usda");
    if(!stage) {
        std::cout << "Open USD stage failed!" << std::endl;
        return -1;
    }
    std::cout << "Parse Usd File to Scene Graph begin..." << std::endl;
    universe::UniSettings uni_settings;
    universe::import::util::convert_to_z_up(stage, &uni_settings);
    universe::import::util::find_prefix_to_skip(stage, &uni_settings);
    uni_settings.scale = 1.0;
    uni_settings.export_dir = "/Users/bytedance/Workspace/Filament/glTFSamples/uni";

    std::unique_ptr<universe::sd::UniSceneDescription> uni_sd = std::make_unique<universe::sd::UniSceneDescription>();
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

        universe::sd::UniNode *node = reader->Node();
        reader->ReadNodeData(uni_sd.get(), 0.0);

        //Parent-Child Relationship Creation
        universe::UniPrimReader *parent = reader->Parent();
        if(parent)
        {
            universe::sd::UniNode* parent_node = parent->Node();
            universe::sd::UniNode* child_node = reader->Node();
            if(parent_node && child_node)
            {
                child_node->parent = parent_node;
                parent_node->children.push_back(child_node);
            }
        }
    }
    std::cout << "Parse Usd File to Scene Graph done!" << std::endl;

    return 0;
}