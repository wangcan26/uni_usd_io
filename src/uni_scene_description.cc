#include "uni_scene_description.h"

constexpr static int  kSceneNamePrefxSize = 6;
constexpr static char *kSceneNamePrefix = "scene_";

namespace universe 
{

namespace util 
{

std::unordered_map<int, int> kNumOfDataType = {
{0, 0},
{1, 1},
{2, 2},
{3, 3},
{4, 4},
{5, 4},
{6, 9},
{7, 16}
};

std::unordered_map<int, int> kSizeOfComponentType = {
{0, 0},
{1, sizeof(UniByte)},
{2, sizeof(UniUByte)},
{3, sizeof(UniShort)},
{4, sizeof(UniUShort)},
{5, sizeof(UniUint)},
{6, sizeof(UniFloat)}
};

std::unordered_map<int, int> kStrideOfAttributeType = {
{0, 0}, 
{1, sizeof(UniFloat) * 3},
{2, sizeof(UniFloat) * 3},
{3, sizeof(UniFloat) * 3},
{4, sizeof(UniFloat) * 2},
{5, sizeof(UniFloat) * 4}
};

}

namespace sd
{

UniSceneDescription::UniSceneDescription()
{
    scenes_.clear();
    buffers_.clear();
    nodes_.clear();
    meshes_.clear();
}

UniScene* UniSceneDescription::CreateScene(const std::string& name)
{
    if(name.size() < 1)
    {
        return nullptr;
    }
    UniScene *scene = new UniScene;
    size_t name_size = kSceneNamePrefxSize + name.size() + 1;
    scene->name = (char*)malloc(name_size);
    std::string scene_name = kSceneNamePrefix + name;
    scene_name.copy(scene->name, name_size -1);
    scene->name[name_size-1] = '\0';

    scenes_.push_back(scene);

    std::cout << "Dbug: UniSceneDescription CreateScene " << scene->name << std::endl;
    return scene; 
}

UniBuffer *UniSceneDescription::CreateBuffer(const std::string& uri, const std::string& name)
{
    if(name.size() < 1)
    {
        return nullptr;
    }
    UniBuffer *buffer = new UniBuffer;
    size_t name_size = name.size() + 1;
    buffer->name = (char*) malloc(name_size);
    name.copy(buffer->name, name_size-1);
    buffer->name[name_size-1] = '\0';
    buffer->size = 0;
    std::cout << "Dbug: UniSceneDescription CreateBuffer " << buffer->name << std::endl;
    buffer->uri = uri + std::to_string(buffers_.size()) + ".bin";
    buffers_.push_back(buffer);
    return buffer;
}

UniBuffer *UniSceneDescription::BufferData(UniByte *data, UniSize byte_size, int index)
{
    UniBuffer *buffer = buffers_[index];
    if(!buffer)
    {
        return nullptr;
    }
    UniSize offset = buffer->data.size();
    buffer->data.resize(byte_size + offset);
    /*for(int i = 0; i < byte_size; i++)
    {
        buffer->data.push_back(data[i]);
    }*/
    std::memcpy(&buffer->data[offset], data, byte_size);

    std::cout << "Buffer " << buffer->name << " byteoffset: " << buffer->size;
    buffer->size += byte_size;
    std::cout << " bytelength: " << byte_size << std::endl;    
    return buffer;
}

UniBufferView *UniSceneDescription::CreateBufferView(UniBuffer *buffer, UniSize byteOffset, UniSize byteLength, UniBufferTarget target)
{
    if(!buffer)
    {
        return nullptr;
    }
    UniBufferView* view = new UniBufferView;
    view->buffer = buffer;
    view->offset = byteOffset;
    view->length = byteLength;
    view->target = target;
    view->name = buffer->name;
    UniInt viewCount = views_.size();
    views_[view] = viewCount;
    return view;
}

UniAccessor *UniSceneDescription::CreateAccessor(UniBufferView* view, UniComponentType componentType, UniType type, UniSize count, UniSize byteOffset)
{
    if(!view)
    {
        return nullptr;
    }
    UniAccessor *accessor = new UniAccessor;
    accessor->component_type = componentType;
    accessor->type = type;
    accessor->buffer_view = view;
    accessor->count = count;
    accessor->offset = byteOffset;
    accessor->name = view->name;
    accessor->has_min = accessor->has_max = false;
    UniInt accessorCount = accessors_.size();
    accessors_[accessor] = accessorCount;
    return accessor;
}

UniNode *UniSceneDescription::CreateNode(const std::string& name)
{
    UniNode *node = new UniNode;
    size_t name_size = name.size() + 1;
    node->name = (char*) malloc(name_size);
    name.copy(node->name, name_size-1);
    node->name[name_size-1] = '\0';
    node->parent = nullptr;
    node->mesh = nullptr;
    std::cout << "Dbug: UniSceneDescription CreateNode " << node->name << std::endl;
    UniInt nodeCount = nodes_.size();
    nodes_[node] = nodeCount;
    return node;
}

UniMesh *UniSceneDescription::CreateMesh(UniNode *node)
{
    if(node == nullptr)
    {
        return nullptr;
    }
    UniMesh *mesh = new UniMesh;
    mesh->name = node->name;
    node->mesh = mesh;
    std::cout << "Dbug: UniSceneDescription CreateMesh " << node->name << std::endl;
    UniInt mesh_count = meshes_.size();
    meshes_[mesh] = mesh_count;
    return mesh;
}

UniPrimitive *UniSceneDescription::CreatePrimitive(UniMesh *mesh, UniPrimitiveType type, UniAccessor *indices)
{
    if(mesh == nullptr)
    {
        return nullptr;
    }
    UniPrimitive *primitive = new UniPrimitive;
    primitive->type = type;
    primitive->indices = indices;
    mesh->primitives.push_back(primitive);
    return primitive;
}

UniAttribute *UniSceneDescription::CreateAttribute(UniPrimitive* primitive, UniAccessor *data, UniAttributeType type)
{
    if(!data || !primitive)
    {
        return nullptr;
    }
    UniAttribute *attribute = new UniAttribute;
    attribute->data = data;
    attribute->type = type;
    attribute->name = data->name;
    primitive->attributes.push_back(attribute);
    return attribute;
}

const UniSceneList& UniSceneDescription::GetScenes() const
{
    return scenes_;
}
const UniBufferList& UniSceneDescription::GetBuffers() const
{
    return buffers_;
}

UniInt UniSceneDescription::GetIndexOfNode(UniNode* node){
    if(nodes_.find(node) != nodes_.end())
    {
        return nodes_[node];
    }
    return -1;
}

UniInt UniSceneDescription::GetIndexOfMesh(UniMesh* mesh){
    if(meshes_.find(mesh) != meshes_.end())
    {
        return meshes_[mesh];
    }
    return -1;
}

UniInt UniSceneDescription::GetIndexOfAccessor(UniAccessor *accessor)
{
    if(accessors_.find(accessor) != accessors_.end())
    {
        return accessors_[accessor];
    }
    return -1;
}

UniInt UniSceneDescription::GetIndexOfBufferView(UniBufferView *view)
{
    if(views_.find(view) != views_.end())
    {
        return views_[view];
    }
    return -1;
}

UniInt UniSceneDescription::GetIndexOfBuffer(UniBuffer *buffer)
{
    return 0;
}

UniSize UniSceneDescription::GetNodeSize()
{
    return nodes_.size();
}

UniSize UniSceneDescription::GetMeshSize()
{
    return meshes_.size();
}

UniSize UniSceneDescription::GetAccessorSize()
{
    return accessors_.size();
}

UniSize UniSceneDescription::GetBufferViewSize()
{
    return views_.size();
}

UniSize UniSceneDescription::GetBufferSize()
{
    return buffers_.size();
}

} //sd

}