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
typedef char UniByte;
typedef ushort UniUShort;

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

typedef enum class UniBufferTarget
{
    INVALID,
    INDICES,
    VERTICES,
    MAX
}UniBufferTarget;

typedef enum class UniDataFreeMethod {
    NONE,
    FILE_RELEASE,
    MEMORY_FREE,
    MAX
}UniDataFreeMethod;

typedef enum class UniAttributeType {
    INVALID,
    POSITION,
    NORMAL,
    TANGENT,
    TEXCOORD,
    COLOR,
    JOINTS,
    WEIGHTS,
    CUSTOM,
    MAX
}UniAttributeType;

typedef struct UniBuffer
{
    char *name;
    UniSize size; /* in bytes */
    std::string uri;  //url to raw data file
    std::vector<UniByte> data; /* loaded by load_buffers */
    UniDataFreeMethod free_method;
}UniBuffer;

typedef struct UniBufferView
{
    char *name;
    UniBuffer *buffer;
    UniSize offset;
    UniSize length;
    UniSize stride; /* 0 automatically determined by accessor*/
    UniBufferTarget target; /* indices, vertices*/

}UniBufferView;

typedef struct UniAccessor
{
    char *name;
    UniComponentType component_type;
    UniBool normalized;
    UniType  type;
    UniSize  offset;
    UniSize  count;
    UniSize  stride;
    UniBufferView *buffer_view;
    UniBool  has_min;
    UniFloat min[16];
    UniBool  has_max;
    UniFloat max[16];
}UniAccessor;

typedef struct UniAttribute
{
    char *name;
    UniAttributeType type;
    UniInt  index; //Unkown, Todo: How Filament Usd
    UniAccessor *data;
}UniAttribute;

typedef struct UniPrimitive {
    UniPrimitiveType type;
    UniAccessor *indices;
    std::vector<UniAttribute*> attributes;
} UniPrimitive;

typedef struct UniMesh{
    char *name;
    std::vector<UniPrimitive*> primitives;
}UniMesh;

typedef struct UniNode {
    char *name;
    UniNode* parent;
    std::vector<UniNode*> children;
    UniMesh  *mesh;
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
    std::vector<UniNode*> nodes;
}UniScene;

class UniSceneDescription 
{
public:
    UniSceneDescription(){};
    ~UniSceneDescription(){};

    UniScene* CreateScene(const std::string& name);
    UniBuffer *CreateBuffer(const std::string& uri, const std::string& name);
    UniBuffer *BufferData(UniByte* data, UniSize byte_size, int index = 0);
    UniBufferView *CreateBufferView(UniBuffer *buffer, UniSize byteOffset, UniSize byteLength, UniBufferTarget target);
    UniAccessor *CreateAccessor(UniBufferView* view, UniComponentType componentType, UniType type, UniSize count, UniSize byteOffset = 0);
    UniNode *CreateNode(const std::string& name, UniScene *scene = nullptr);
    UniMesh *CreateMesh(UniNode *node);
    UniPrimitive *CreatePrimitive(UniMesh *mesh, UniPrimitiveType type = UniPrimitiveType::POINTS, UniAccessor *indices = nullptr);
    UniAttribute *CreateAttribute(UniPrimitive *primitive, UniAccessor *data, UniAttributeType type);

private:
    std::vector<UniScene *> scenes_;
    std::vector<UniBuffer*> buffers_;
    //map of usd stage and scene
};

} //sd

}