#pragma once 
#include "uni_prerequisites.h"
#include "uni_scene_description.h"

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
    UniInt  byteOffset;
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
};

class GLTFJsonExporter
{
public:
    GLTFJsonExporter(){};
    ~GLTFJsonExporter(){};

    void GatherSceneDescription(sd::UniSceneDescription *sd);

    void finalize();

private:
    
    GLTFJsonizer    gltf_;
};

}//universe