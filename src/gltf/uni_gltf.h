#pragma once 
#include "uni_prerequisites.h"
#include "uni_scene_description.h"
#include "uni_settings.h"

namespace universe 
{

typedef struct GLTFBuffer
{
    UniSize byte_length;
    std::string file_name;
}GLTFBuffer;

typedef struct GLTFBufferView
{
    UniInt  buffer;
    UniSize byte_length;
    UniSize  byte_offset;
    UniInt  target;
}GLTFBufferView;

typedef struct GLTFAccessor
{
    UniInt  buffer_view;
    UniInt  component_type;
    UniInt  count;
    std::string type;
    UniSize range;
    std::vector<UniInt>  max;
    std::vector<UniInt>  min;
}GLTFAccessors;

typedef struct GLTFAttribute
{
    std::string semantic;
    UniInt   index;
}GLTFAttribute;

typedef struct GLTFPrimitive
{
    std::vector<GLTFAttribute> attributes;
    UniInt indices_index;
}GLTFPrimitive;

typedef struct GLTFMesh
{
    std::string name;
    std::vector<GLTFPrimitive> primitives;
}GLTFMesh;

typedef struct GLTFNode
{
    UniInt   mesh;
    UniInt   parent;
    std::vector<UniInt> children;
    std::string name;
}GLTFNode;

typedef struct GLTFScene
{
    std::string name;
    std::vector<UniInt> nodes;
}GLTFScene;

typedef struct GLTFJsonizer
{
    //Todo: asset, extension etc

    //default scene
    UniInt  scene;
    std::vector<GLTFScene> scenes;
    std::vector<GLTFNode> nodes;
    std::vector<GLTFMesh> meshes;
    std::vector<GLTFAccessor> accessors;
    std::vector<GLTFBufferView> buffer_views;
    std::vector<GLTFBuffer>  buffers;
}GLTFJsonizer;

class GLTFJsonExporter
{
public:
    GLTFJsonExporter(const UniSettings& settings);
    ~GLTFJsonExporter(){};

    bool GatherSceneDescription(sd::UniSceneDescription *sd);

    void Finalize();

    bool Validate(const std::string &gltf_file);

private:
    GLTFJsonizer    gltf_;
    std::string     path_ = "";
    std::string     file_name_ = "";
    sd::UniBufferList   buffers_;
};

}//universe