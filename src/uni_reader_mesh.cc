#include "uni_reader_mesh.h"

namespace universe {

UniMeshReader::UniMeshReader(const pxr::UsdPrim &prim, const UniSettings& settings)
    : UniGeomReader(prim, settings),
    mesh_prim_(prim),
    is_left_handed_(false)
{

}

void UniMeshReader::CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime)
{
    node_ = uni_sd->CreateNode(name_);
}

void UniMeshReader::ReadNodeData(sd::UniSceneDescription *uni_sd, double motionSampleTime)
{
    if(!mesh_prim_){
        std::cout << "Dbug: UniMeshReader ReadNodeData null usd mesh prim " << std::endl;
        return;
    }
    pxr::TfToken orientation;
    mesh_prim_.GetOrientationAttr().Get(&orientation);
    if(orientation == pxr::UsdGeomTokens->leftHanded) {
        is_left_handed_ = true;
    }
    
    mesh_prim_.GetFaceVertexIndicesAttr().Get(&face_inidices_, motionSampleTime);
    mesh_prim_.GetFaceVertexCountsAttr().Get(&face_counts_, motionSampleTime);
    mesh_prim_.GetPointsAttr().Get(&positions_, motionSampleTime);
    std::cout << "Dbug: UniMeshReader ReadNodeData " << name_ << " Info: " << std::endl;
    std::cout << "Dbug: face counts: " << face_counts_.size() << std::endl;
    std::cout << "Dbug: face indices: " << face_inidices_.size() << std::endl;
    int i = 0;
    for(auto indice : face_inidices_) {
        std::cout << "Dbug: face indice " << i  << " : " << indice << std::endl;
        i = i+1;
    }
    std::cout << "Dbug: face position size: " << positions_.size() << " capacity: " << positions_.capacity() << std::endl;
    UniByte *posdata = (UniByte*)positions_.data();
    for(int i = 0 ; i < positions_.size() * 3; i++)
    {
        if( i % 3 == 0)
        {
            std::cout << "Dbug: face position" << i/3 << ":"; 
        }
        std::cout << " " <<  *(UniFloat*)posdata;
        posdata = posdata + sizeof(UniFloat);
        if( i % 3 == 2)
        {
            std::cout << ";" << std::endl;
        }
    }
    //USD Mesh data ->1 gltf Buffer -> 
    //2 gltf BufferView -> 2 gltf Accessors -> 1 indices and 1 pos attribute ->  1 gltf Mesh Primitive -> 1 gltf Mesh

    //Buffer Data Positions
    UniSize posByteSize = positions_.size() * 3 * sizeof(UniFloat);
    sd::UniBuffer *buffer = uni_sd->BufferData((UniByte*)positions_.data(), posByteSize);
    //Create BufferView for Position
    sd::UniBufferView *pos_view = uni_sd->CreateBufferView(buffer, buffer->size - posByteSize, posByteSize, sd::UniBufferTarget::VERTICES);
    pos_view->name = node_->name;
    //Create Accessor for Pos BufferView
    sd::UniAccessor *pos_accessor = uni_sd->CreateAccessor(pos_view, sd::UniComponentType::R_32F, sd::UniType::VEC3, positions_.size());

    //Buffer Data Indices
    std::vector<UniUShort> indices;
    for(auto indice : face_inidices_)
    {
        indices.push_back((UniUShort)indice);
    }
    UniSize indiceByteSize = face_inidices_.size() * sizeof(UniUShort);
    uni_sd->BufferData((UniByte*)indices.data(), indiceByteSize);
    //Create BufferView for Indices 
    sd::UniBufferView *indice_view = uni_sd->CreateBufferView(buffer, buffer->size - indiceByteSize, indiceByteSize, sd::UniBufferTarget::INDICES);
    indice_view->name = node_->name;
    //Create Accessor for Indice BufferView
    sd::UniAccessor *indice_accessor = uni_sd->CreateAccessor(indice_view, sd::UniComponentType::R_16U, sd::UniType::VEC3, positions_.size());

    sd::UniMesh *mesh = uni_sd->CreateMesh(node_);

    //Create Mesh Primitive
    sd::UniPrimitive *primitive = uni_sd->CreatePrimitive(mesh, sd::UniPrimitiveType::POINTS, indice_accessor);
    //Create Pos Attribute
    uni_sd->CreateAttribute(primitive, pos_accessor, sd::UniAttributeType::POSITION);
    std::cout << "Mesh " << node_->name << " Read Data Done!" << std::endl;
    UniXformReader::ReadNodeData(uni_sd, motionSampleTime);
}  

}