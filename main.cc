#include <iostream>
#include <pxr/usd/usd/stage.h>
#include <uni_scene_description.h>
#include <uni_reader_stage.h>
#include "uni_settings.h"
#include "gltf/uni_gltf.h"
#include <memory>
#include <filesystem>

namespace fs = std::filesystem;

int main() 
{
    std::cout << "Hello Universe IO " << std::endl;
    //Current working directory
    std::string work_path = fs::current_path().u8string();
    std::cout << "Current working directory: " << work_path << std::endl;
    std::string usd_file = "";
#ifdef UNI_PLATFORM_WINDOWS
    usd_file = work_path + "\\Release\\data\\HelloUSD.usda";
#else
    usd_file = work_path + "/data/HelloUSD.usda";
#endif 

    pxr::UsdStageRefPtr stage = pxr::UsdStage::Open(usd_file);
    if(!stage) {
        std::cout << "Open USD stage failed!" << std::endl;
        return -1;
    }
    std::cout << "Parse Usd File to Scene Graph begin..." << std::endl;
    universe::UniSettings uni_settings;
    universe::import::util::convert_to_z_up(stage, &uni_settings);
    universe::import::util::find_prefix_to_skip(stage, &uni_settings);
    uni_settings.scale = 1.0;
    uni_settings.export_dir = work_path;

    std::unique_ptr<universe::sd::UniSceneDescription> uni_sd = std::make_unique<universe::sd::UniSceneDescription>();
    std::unique_ptr<universe::UniStageReader> archive = std::make_unique<universe::UniStageReader>(stage, uni_settings);
    archive->CollectReaders(uni_sd.get());

    //archive sort readers
    //...

    for(universe::UniPrimReader *reader : archive->Readers()){
        if(!reader){
            continue;
        }

        reader->CreateNode(uni_sd.get(), 0.0);
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
            
            if(parent_node && node)
            {
                node->parent = parent_node;
                parent_node->children.push_back(node);
            }
        }else{
            universe::sd::UniScene *scene = archive->CurrentScene();
            node->parent = nullptr;
            scene->nodes.push_back(node);
        }
    }
    std::cout << "Parse Usd File to Scene Graph done!" << std::endl;

    universe::GLTFJsonExporter expoter;
    expoter.GatherSceneDescription(uni_sd.get());

#if UNI_PLATFORM_WINDOWS 
    system("PAUSE");
#endif 

    return 0;
}