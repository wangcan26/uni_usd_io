#pragma once 
#include "uni_prerequisites.h"

namespace universe 
{

typedef size_t UniSize;
typedef long long int UniSSize;
typedef float UniFloat;
typedef int UniInt;
typedef unsigned int UniUint;
typedef int  UniBool;

namespace sd
{
    
typedef enum class UniComponentType
{
    Invalid,
    R_8,     /*BYTE*/
    R_8U,    /* UNSIGNED BYTE*/
    R_16,    /* SHORT */
    R_16U,   /* UNSIGNED_SHORT */
    R_32U,   /* UNSIGNED_INT*/
    R_32F,   /* FLOAT */
    MAX
}UniComponentType;

typedef enum class UniType
{
    INVALID,
    SCALAR,
    VEC2,
    VEC3,
    VEC4,
    MAT2,
    MAT3,
    MAT4,
    MAX
}UniType;

typedef enum class UniPrimitiveType
{
    POINTS,
    LINES,
    LINE_LOOP,
    LINE_STRIP,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN,
    MAX
}UniPrimitiveType;

typedef struct UniAccessor
{
    char *name;
    UniComponentType component_type;
    UniBool normalized;
    UniType  type;
    UniSize  offset;
    UniSize  count;
    UniSize  stride;
    UniBool  has_min;
    UniFloat min[16];
    UniBool  has_max;
    UniFloat max[16];
}UniAccessor;

typedef struct UniPrimitive {
    UniPrimitiveType type;


} UniPrimitive;

typedef struct UniMesh{
    char *name;

}UniMesh;

typedef struct UniNode {
    char *name;
    UniNode* parent;
    std::vector<UniNode*> children;
    UniBool  has_translation;
    UniBool  has_rotation;
    UniBool  has_scale;
    UniBool  has_matrix;
    UniFloat translation[3];
    UniFloat rotation[4];
    UniFloat scale[3];
    UniFloat matrix[16];
}UniNode;

typedef struct UniScene {
    char *name;
    std::vector<UniNode*> nodes;;
}UniScene;

class UniSceneDescription 
{
public:
    UniSceneDescription(){};
    ~UniSceneDescription(){};

    UniScene* CreateScene(const std::string& name);
    UniNode *CreateNode(const std::string& name, UniScene *scene = nullptr);

private:
    std::vector<UniScene *> scenes_;
    //map of usd stage and scene
};

} //sd

}