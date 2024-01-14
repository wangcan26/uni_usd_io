#include "uni_gltf.h"
#include "nlohmann/json.hpp"
#include <filesystem>
#include <fstream>
#include <stdio.h>


namespace universe
{
using namespace sd;

namespace util 
{

static std::unordered_map<int, std::string> kMapOfAttributeSemantic = {
    {0, "INVALID"},
    {1, "POSITION"},
    {2, "NORMAL"}
};

static std::unordered_map<int, UniInt> kMapOfComponentType = {
    {0, -1},
    {1, 5120},
    {2, 5121},
    {3, 5122},
    {4, 5123},
    {5, 5125},
    {6, 5126},
};

static std::unordered_map<int, std::string> kMapOfDataType = {
    {0, "INVALID"},
    {1, "SCALAR"},
    {2, "VEC2"},
    {3, "VEC3"},
    {4, "VEC4"},
    {5, "MAT2"},
    {6, "MAT3"},
    {7, "MAT4"}
};

static std::unordered_map<int, UniInt> kMapOfBufferTarget = {
    {0, -1}, 
    {1, 34963},
    {2, 34962}  
};

static bool GatherUniBuffer(UniBuffer *uni_buffer, GLTFBuffer *buffer, UniInt index)
{
    UniSize byte_size = uni_buffer->size;
    buffer->byte_length = byte_size;
    buffer->file_name = uni_buffer->uri;
    return true;
}

static bool GatherUniBufferView(UniBufferView *uni_view, GLTFBufferView *view, sd::UniSceneDescription *sd, GLTFJsonizer *gltf)
{
    view->buffer = sd->GetIndexOfBuffer(uni_view->buffer);
    if(view->buffer == -1){
        return false;
    }
    GLTFBuffer buffer;
    GatherUniBuffer(uni_view->buffer, &buffer, view->buffer);
    gltf->buffers[view->buffer] = buffer;
    view->byte_length = uni_view->length;
    view->byte_offset = uni_view->offset;
    std::cout << "DBug: GatherUniBufferView offset: " << uni_view->offset << std::endl;
    view->target = kMapOfBufferTarget[static_cast<int>(uni_view->target)];
    return true;
}

static bool GatherUniAccessor(UniAccessor *uni_accessor, GLTFAccessor *accessor, sd::UniSceneDescription *sd, GLTFJsonizer *gltf)
{
    UniInt view_index = sd->GetIndexOfBufferView(uni_accessor->buffer_view);
    if(view_index == -1) {
        return false;
    }
    GLTFBufferView view;
    if(!GatherUniBufferView(uni_accessor->buffer_view, &view, sd, gltf))
    {
        return false;
    }
    gltf->buffer_views[view_index] = view;
    accessor->buffer_view = view_index;
    accessor->component_type = kMapOfComponentType[static_cast<int>(uni_accessor->component_type)];
    accessor->count = uni_accessor->count;
    accessor->type = kMapOfDataType[static_cast<int>(uni_accessor->type)];
    int numComponent = kNumOfDataType[static_cast<int>(uni_accessor->type)];
    if(uni_accessor->has_max) {
        for(int i = 0; i < numComponent; i++)
        {
            accessor->max.push_back(uni_accessor->max[i]);
        }
    }
    if(uni_accessor->has_min) {
        for(int i = 0; i < numComponent; i++)
        {
            accessor->min.push_back(uni_accessor->min[i]);
        }
    }
    
    return true;
}

static bool GatherUniPrimitive(UniPrimitive *uni_primitive, GLTFPrimitive *primitive, sd::UniSceneDescription *sd, GLTFJsonizer *gltf)
{
    UniInt indices_index = sd->GetIndexOfAccessor(uni_primitive->indices);
    if(indices_index == -1)
    {
        return false;
    }
    primitive->indices_index = indices_index;
    GLTFAccessor indices_accesor;
    if(!GatherUniAccessor(uni_primitive->indices, &indices_accesor, sd, gltf))
    {
        return false;
    }
    gltf->accessors[indices_index] = indices_accesor;
    for(auto& uni_attribute : uni_primitive->attributes)
    {
        GLTFAttribute attribute;
        attribute.semantic = kMapOfAttributeSemantic[static_cast<int>(uni_attribute->type)];
        attribute.index = sd->GetIndexOfAccessor(uni_attribute->data);
        GLTFAccessor attribute_accessor;
        if(!GatherUniAccessor(uni_attribute->data, &attribute_accessor, sd, gltf))
        {
            return false;
        }
        gltf->accessors[attribute.index] = attribute_accessor;
        primitive->attributes.push_back(attribute);
    }
    return true;
}

static bool GatherUniMesh(UniMesh* uni_mesh, sd::UniSceneDescription *sd, GLTFJsonizer *gltf, UniInt index)
{
    GLTFMesh mesh;
    mesh.name = uni_mesh->name;
    for(auto& uni_primitive : uni_mesh->primitives)
    {
        GLTFPrimitive primitive;
        if(!GatherUniPrimitive(uni_primitive, &primitive, sd, gltf))
        {
            return false;
        }
        mesh.primitives.push_back(primitive);
    }
    gltf->meshes[index] = mesh;
    return true;
}

static bool GatherUniNode(UniNode *uni_node, sd::UniSceneDescription *sd, GLTFJsonizer *gltf, UniInt index)
{
    GLTFNode node;
    node.name = uni_node->name;
    node.parent = sd->GetIndexOfNode(uni_node->parent);
    node.mesh = sd->GetIndexOfMesh(uni_node->mesh);
    if(node.mesh > -1)
    {
        if(!GatherUniMesh(uni_node->mesh, sd, gltf, node.mesh))
        {
            return false;
        }
    }
    gltf->nodes[index] = node;
    for(auto& child : uni_node->children)
    {
        UniInt child_index = sd->GetIndexOfNode(child);
        if(child_index == -1)
        {
            return false;
        }
        if(!GatherUniNode(child, sd, gltf, child_index))
        {
            return false;
        }
    }
    return true;
}

static bool GatherUniScene(const UniScene *uni_scene, GLTFScene *scene, sd::UniSceneDescription *sd, GLTFJsonizer *gltf)
{
    scene->name = uni_scene->name;
    for(auto& uni_node : uni_scene->nodes)
    {
        UniInt node_index = sd->GetIndexOfNode(uni_node);
        if(node_index == -1) {
            return false;
        }
        scene->nodes.push_back(node_index);
        if(!GatherUniNode(uni_node, sd, gltf, node_index))
        {
            return false;
        }
    }
    return true;
}

}//util


GLTFJsonExporter::GLTFJsonExporter(const UniSettings& settings)
    :path_(settings.export_dir),
    file_name_(settings.export_file_name)
{
    gltf_.scene = -1;
}

bool GLTFJsonExporter::GatherSceneDescription(sd::UniSceneDescription *sd)
{
    auto& scenes = sd->GetScenes();
    std::cout << "Dbug: GLTFJsonExporter Gather scene count : " << scenes.size() << std::endl;
    if(scenes.size() > 0)
    {
        gltf_.scene = 0;
    }
    gltf_.nodes.resize(sd->GetNodeSize());
    gltf_.meshes.resize(sd->GetMeshSize());
    gltf_.accessors.resize(sd->GetAccessorSize());
    gltf_.buffer_views.resize(sd->GetBufferViewSize());
    gltf_.buffers.resize(sd->GetBufferSize());
    for(auto& uni_scene : scenes)
    {
        //Gather info for GLTFScene from UniScene
        GLTFScene scene;
        util::GatherUniScene(uni_scene, &scene, sd, &gltf_);
        gltf_.scenes.push_back(scene);
    }
    buffers_ = sd->GetBuffers();
    return true;

}

void GLTFJsonExporter::finalize()
{
    //convert gltf to json string
    nlohmann::json jroot;
    //scene
    if(gltf_.scene > -1)
    {
        jroot["scene"] = gltf_.scene;
    }
    //scenes
    if(gltf_.scenes.size() > 0)
    {
        nlohmann::json jscenes;
        for(auto& scene : gltf_.scenes)
        {
            nlohmann::json jscene;
            if(scene.name.size() > 0)
            {
                jscene["name"] = scene.name;
            }
            if(scene.nodes.size() > 0) {
                jscene["nodes"] = nlohmann::json(scene.nodes);
            }
            jscenes.push_back(jscene);
        }
        jroot["scenes"] =jscenes;
    }
    //nodes
    if(gltf_.nodes.size() > 0) 
    {
        nlohmann::json jnodes;
        for(auto& node : gltf_.nodes)
        {
            nlohmann::json jnode;
            if(node.name.size() > 0)
            {
                jnode["name"] = node.name;
            }
            if(node.parent > -1)
            {
                jnode["parent"] = node.parent;
            }
            if(node.mesh > -1) {
                jnode["mesh"] = node.mesh;
            }
            if(node.children.size() > 0)
            {
                jnode["children"] = nlohmann::json(node.children);
            }
            jnodes.push_back(jnode);
        }
        jroot["nodes"] = jnodes;
    }
    //meshes 
    if(gltf_.meshes.size() > 0)
    {
        nlohmann::json jmeshes;
        for(auto& mesh : gltf_.meshes) {
            nlohmann::json jmesh;
            jmesh["name"] = mesh.name;
            nlohmann::json jprimitives;
            for(auto& primtive : mesh.primitives)
            {
                nlohmann::json jprimitive;
                jprimitive["indices"] = primtive.indices_index;
                nlohmann::json jattributes;
                for(auto& attribute : primtive.attributes)
                {
                    jattributes[attribute.semantic] = attribute.index;
                }
                jprimitive["attributes"] = jattributes;
                jprimitives.push_back(jprimitive);
            }
            jmesh["primitives"] = jprimitives;
            jmeshes.push_back(jmesh);
        }
        jroot["meshes"] = jmeshes;
    }
    //accessors
    if(gltf_.accessors.size() > 0) {
        nlohmann::json jaccessors;
        for(auto& accessor : gltf_.accessors) {
            nlohmann::json jaccessor;
            jaccessor["bufferView"] = accessor.buffer_view;
            jaccessor["componentType"] = accessor.component_type;
            jaccessor["count"] = accessor.count;
            jaccessor["type"] = accessor.type;
            if(accessor.max.size() > 0)
            {
                jaccessor["max"] = accessor.max;
            }
            if(accessor.min.size() > 0)
            {
                jaccessor["min"] = accessor.min;
            }
            jaccessors.push_back(jaccessor);
        }
        jroot["accessors"] = jaccessors;
    }
    //buffer views
    if(gltf_.buffer_views.size() > 0) {
        nlohmann::json jviews;
        for(auto& view: gltf_.buffer_views) {
            nlohmann::json jview;
            jview["buffer"] = view.buffer;
            jview["byteLength"] = view.byte_length;
            jview["byteOffset"] = view.byte_offset;
            jview["target"] = view.target;
            jviews.push_back(jview);
        }
        jroot["bufferViews"] = jviews;
    }
    //buffers
    if(gltf_.buffers.size() > 0)
    {
        nlohmann::json jbuffers;
        for(auto& buffer : gltf_.buffers)
        {
            nlohmann::json jbuffer;
            jbuffer["byteLength"] = buffer.byte_length;
            jbuffer["uri"] = buffer.file_name;
            jbuffers.push_back(jbuffer);
        }
        jroot["buffers"] = jbuffers;
    }
    std::cout << "DBug: GLTFJsonExporter finalize : " << std::endl;
    std::string json_content = jroot.dump();
    std::cout << json_content << std::endl;
    //write json and buffer bin
    using namespace std::filesystem;
    path p(path_);
    if(!exists(p))
    {
        std::cout << "DBug: GLTFJsonExporter work path not exist! " << std::endl;
    }
    std::string finalize_path = path_;
#ifdef UNI_PLATFORM_WINDOWS
    finalize_path = finalize_path + "\\out";
#else 
    finalize_path = finalize_path + "/out";
#endif 

    path fl_p(finalize_path);
    if(!exists(fl_p))
    {
        std::cout << "DBug: GLTFJsonExporter finalize path not exist! " << std::endl;
    }
    
    if(create_directory(fl_p))
    {
        std::cout << "DBug: GLTFJsonExporter finalize path create success! " << std::endl;
    }
    //write json
    path fp = fl_p / (file_name_ + ".gltf");
    std::ofstream ofs(fp);
    ofs << json_content;
    ofs.close();
    
    //write bins
    int buffer_count = 0;
    for(auto& buffer : buffers_)
    {        
        fp = fl_p / (file_name_ + std::to_string(buffer_count) + ".bin");
        std::cout << "DBug: GLTFJsonExporter finalize write bin file: " << fp.generic_string() << std::endl;
        FILE* fp_handle = NULL;
        fp_handle = fopen(fp.generic_string().c_str(), "wb");
        if (fp_handle)
        {
            sd::UniBuffer* buffer = buffers_[buffer_count];
            //trick
            UniFloat *pos = (UniFloat*)buffer->data.data();
            pos[7] = 1;
            pos[8] = 0;
            //check buffer data
            UniByte *byte = buffer->data.data();
            for(int i = 0; i < 3 * 3; i++)
            {
                if( i % 3 == 0)
                {
                    std::cout << "Dbug: check buffer position" << i/3 << ":"; 
                }
                UniFloat pos = ((UniFloat*)byte)[i];
                std::cout << pos;
                if( i % 3 == 2)
                {
                    std::cout << ";" << std::endl;
                }
            }
            byte = buffer->data.data() + 36;
            for(int i = 0; i < 3; i++)
            {
                if( i % 3 == 0)
                {
                    std::cout << "Dbug: check buffer indices" << ":"; 
                }
                UniUShort indice = ((UniUShort*)byte)[i];
                std::cout << indice;
                if( i % 3 == 2)
                {
                    std::cout << ";" << std::endl;
                }
            }
            fwrite(buffer->data.data(), buffer->size, 1, fp_handle);
        }
        fclose(fp_handle);
        buffer_count++;
    }
;}

} //universe