#include "uni_gltf.h"

namespace universe
{
using namespace sd;

namespace util 
{
typedef struct UniTree {
    std::vector<GLTFNode> nodes;
}UniTree;

static UniTree kUniTree;

static void ClearUniTree()
{
    kUniTree.nodes.clear();
}

static void GatherUniNode(UniNode *uni_node, sd::UniSceneDescription *sd)
{
    GLTFNode node;
    node.name = uni_node->name;
    node.parent = sd->GetIndexOfNode(uni_node->parent);
    node.mesh = sd->GetIndexOfMesh(uni_node->mesh);
    kUniTree.nodes.push_back(node);
    for(auto& child : uni_node->children)
    {
        GatherUniNode(child, sd);
    }
}

static void GatherUniScene(const UniScene *uni_scene, GLTFScene *scene, sd::UniSceneDescription *sd)
{
    scene->name = uni_scene->name;
    for(auto& uni_node : uni_scene->nodes)
    {
        UniInt node_index = sd->GetIndexOfNode(uni_node);
        scene->nodes.push_back(node_index);
        GatherUniNode(uni_node, sd);
    }
}

}//util


void GLTFJsonExporter::GatherSceneDescription(sd::UniSceneDescription *sd)
{
    auto& scenes = sd->GetScenes();
    std::cout << "Dbug: GLTFJsonExporter Gather scene count : " << scenes.size() << std::endl;
    if(scenes.size() > 1)
    {
        gltf_.scene = 0;
    }
    util::ClearUniTree();
    for(auto& uni_scene : scenes)
    {
        //Gather info for GLTFScene from UniScene
        GLTFScene scene;
        util::GatherUniScene(uni_scene, &scene, sd);
        gltf_.scenes.push_back(scene);
    }
    gltf_.nodes = util::kUniTree.nodes;

}

void GLTFJsonExporter::finalize()
{
    
}

} //universe