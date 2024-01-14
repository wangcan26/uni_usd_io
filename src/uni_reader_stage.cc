#include "uni_reader_stage.h"
#include <pxr/pxr.h>
#include <pxr/usd/usd/prim.h>
#include <pxr/usd/usdGeom/xform.h>
#include <pxr/usd/usdGeom/mesh.h>
#include "uni_reader_geom.h"
#include "uni_reader_mesh.h"

namespace universe {

namespace helper {

static bool MergeWithParent(UniPrimReader *reader)
{
    UniXformReader *xform_reader = dynamic_cast<UniXformReader *>(reader);

    if(!xform_reader)
    {
        return false;
    }

    if(xform_reader->UseParentXform()){
        return false;
    }

    /* Only merge if the parent is an Xform. */
    if(!xform_reader->Prim().GetParent().IsA<pxr::UsdGeomXform>()) {
        return false;
    }

    /* Don't merge Xform prims*/
    if(xform_reader->Prim().IsA<pxr::UsdGeomXform>()){
        return false;
    }

    if(xform_reader->PrimHasXformOps())
    {
        return false;
    }

    xform_reader->SetUsdParentXform(true);


    return true;
}

}

UniStageReader::UniStageReader(pxr::UsdStageRefPtr stage, const UniSettings &settings):
    stage_(stage),
    cur_scene_(nullptr),
    settings_(settings)
{

}

UniStageReader::~UniStageReader()
{

}

bool UniStageReader::Valid() const
{
    return stage_;
}

UniPrimReader* UniStageReader::CreateReaderIfAllowed(const pxr::UsdPrim &prim)
{
    if(prim.IsA<pxr::UsdGeomMesh>()) {
        //std::cout << "Reader of type geom mesh" << std::endl;
        return new UniMeshReader(prim, settings_);
    }
    if(prim.IsA<pxr::UsdGeomImageable>()){
        //std::cout << "Reader of type xform" << std::endl;
        return new UniXformReader(prim, settings_);
    }
    return nullptr;
}

void UniStageReader::CollectReaders(sd::UniSceneDescription *sd)
{
    if(!Valid())
    {
        return;
    }

    ClearReaders();

    //Root Prim
    pxr::UsdPrim root = stage_->GetPseudoRoot();
    //obtain root prim name
    std::string name = root.GetName().GetString();
    std::cout << "USD Stage's name is " << name << std::endl;
    cur_scene_ = sd->CreateScene(name);
    sd->CreateBuffer(settings_.export_file_name, settings_.export_file_name);

    CollectReaders(sd, root);
}

UniPrimReader *UniStageReader::CollectReaders(sd::UniSceneDescription* sd, const pxr::UsdPrim &prim)
{
    pxr::Usd_PrimFlagsPredicate filter_predicate = pxr::UsdPrimDefaultPredicate;

    pxr::UsdPrimSiblingRange children = prim.GetFilteredChildren(filter_predicate);
    int child_size = 0;
    for(const auto &child_prim : children) {
        child_size++;
    }

    std::vector<UniPrimReader *> child_readers;
    for(const auto &child_prim : children) {
        const std::string name = child_prim.GetName().GetString();
        //std::cout << "USD Prim's name is " << name << std::endl;
        if(UniPrimReader *child_reader = CollectReaders(sd, child_prim)) {
            child_readers.push_back(child_reader);
        }
    }

    if(prim.IsPseudoRoot()){
        return nullptr;
    }

    if(!settings_.skip_prefix.IsEmpty()) {
        if(settings_.skip_prefix.HasPrefix(prim.GetPath())) {
            return nullptr;
        }
    }

    //if a prim is not a xform or a scope, it can be merged with the parent in terms of xform
    if(child_readers.size() == 1)
    {
        UniPrimReader *child_reader = child_readers.front();
        if(helper::MergeWithParent(child_reader))
        {
            std::cout << "USD Prim  " << child_reader->Name() << " merge with parent " << 
                child_reader->Prim().GetParent().GetName().GetString() << std::endl;
            return child_reader;
        }
    }
    

    UniPrimReader *reader = CreateReaderIfAllowed(prim);
    if(!reader)
    {
        return nullptr;
    }
    
    readers_.push_back(reader);

    for(UniPrimReader *child_reader : child_readers){
        child_reader->Parent(reader);
    }

    return reader;
}

void UniStageReader::ClearReaders()
{
    for(UniPrimReader *reader : readers_)
    {
        if(!reader){
            continue;
        }

        if(reader)
        {
            delete reader;
        }
    }

    readers_.clear();
}

sd::UniScene *UniStageReader::CurrentScene() const
{
    return cur_scene_;
}

}