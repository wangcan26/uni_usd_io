#include "uni_scene_description.h"

constexpr static int kSceneNamePrefxSize = 6;
constexpr static char *kSceneNamePrefix = "scene_";

namespace universe 
{

namespace sd
{

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
    std::cout << "Dbug: UniSceneDescription CreateBuffer " << buffer->name << std::endl;
    buffer->uri = uri;
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
    for(int i = 0; i < byte_size; i++)
    {
        buffer->data.push_back(*(UniByte*)data);
        data = data + sizeof(UniByte);
    }
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

} //sd

}