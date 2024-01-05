#pragma once 
#include <pxr/usd/usd/prim.h>
#include "uni_scene_description.h"
#include "uni_settings.h"

namespace universe 
{
class UniPrimReader
{
public:
    UniPrimReader(const pxr::UsdPrim &prim, const UniSettings& settings);
    virtual ~UniPrimReader();

    const pxr::UsdPrim &Prim() const;

    virtual bool Valid() const;

    virtual void CreateNode(sd::UniSceneDescription *uni_sd, double motionSampleTime = 0.0) = 0;
    virtual void ReadNodeData(sd::UniSceneDescription *uni_sd, double motionSampleTime) = 0;
    
    sd::UniNode *Node() const;

    UniPrimReader *Parent() const {
        return parent_;
    }

    void Parent(UniPrimReader *parent) {
        parent_ = parent;
    }

    const std::string &Name() const 
    {
        return name_;
    }

    const std::string &PrimPath() const 
    {
        return prim_path_;
    }
protected:
    std::string name_ = "";
    std::string prim_path_ = "";
    pxr::UsdPrim prim_;
    UniPrimReader *parent_;
    sd::UniNode *node_;
    UniSettings settings_;
};

}