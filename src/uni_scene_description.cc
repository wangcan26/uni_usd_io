#include "uni_scene_description.h"
#include <iostream>

constexpr static int kSceneNamePrefxSize = 6;
constexpr static char *kSceneNamePrefix = "scene_";

namespace universe 
{

UniScene* UniSceneDescription::CreateScene(const std::string& name)
{
    UniScene *scene = new UniScene;

    if(name.size() < 1)
    {
        return nullptr;
    }

    size_t name_size = kSceneNamePrefxSize + name.size() + 1;
    scene->name = (char*)malloc(name_size);
    std::string scene_name = kSceneNamePrefix + name;
    scene_name.copy(scene->name, name_size -1);
    scene->name[name_size-1] = '\0';

    scenes_.push_back(scene);

    std::cout << "Dbug: UniSceneDescription CreateScene " << scene->name << std::endl;
    return scene; 
}

UniNode *UniSceneDescription::CreateNode(const std::string& name, UniScene *scene)
{
    UniNode *node = new UniNode;
    size_t name_size = name.size() + 1;
    node->name = (char*) malloc(name_size);
    name.copy(node->name, name_size-1);
    node->name[name_size-1] = '\0';
    std::cout << "Dbug: UniSceneDescription CreateNode " << node->name << std::endl;
    if(scene)
    {
        scene->nodes.push_back(node);
    }

    return nullptr;
}

}