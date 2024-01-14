#pragma once 
#include "uni_prerequisites.h"

namespace universe 
{

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
    UniInt  index; //Unkown, Todo: How Filament Use
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

using UniSceneList = std::vector<UniScene*>;
using UniBufferList = std::vector<UniBuffer*>;
class UniSceneDescription 
{
public:
    UniSceneDescription();
    ~UniSceneDescription(){};

    UniScene* CreateScene(const std::string& name);
    UniBuffer *CreateBuffer(const std::string& uri, const std::string& name);
    UniBuffer *BufferData(UniByte* data, UniSize byte_size, int index = 0);
    UniBufferView *CreateBufferView(UniBuffer *buffer, UniSize byteOffset, UniSize byteLength, UniBufferTarget target);
    UniAccessor *CreateAccessor(UniBufferView* view, UniComponentType componentType, UniType type, UniSize count, UniSize byteOffset = 0);
    UniNode *CreateNode(const std::string& name);
    UniMesh *CreateMesh(UniNode *node);
    UniPrimitive *CreatePrimitive(UniMesh *mesh, UniPrimitiveType type = UniPrimitiveType::POINTS, UniAccessor *indices = nullptr);
    UniAttribute *CreateAttribute(UniPrimitive *primitive, UniAccessor *data, UniAttributeType type);

    const UniSceneList& GetScenes() const;
    const UniBufferList& GetBuffers() const;
    UniInt GetIndexOfNode(UniNode* node);
    UniInt GetIndexOfMesh(UniMesh* mesh);
    UniInt GetIndexOfAccessor(UniAccessor *accessor);
    UniInt GetIndexOfBufferView(UniBufferView* view);
    UniInt GetIndexOfBuffer(UniBuffer *buffer);
    UniSize GetNodeSize();
    UniSize GetMeshSize();
    UniSize GetAccessorSize();
    UniSize GetBufferViewSize();
    UniSize GetBufferSize();

private:
    UniSceneList scenes_;
    UniBufferList buffers_;
    
    std::unordered_map<UniNode*, UniInt> nodes_;
    std::unordered_map<UniMesh*, UniInt> meshes_;
    std::unordered_map<UniAccessor*, UniInt> accessors_;
    std::unordered_map<UniBufferView*, UniInt> views_;
};

} //sd

namespace util {

template<typename T>
bool FillAccessorMaxMinValues(sd::UniBufferTarget bufferTarget, sd::UniAttributeType attribute, sd::UniType type, const T* data, int count, float* max, float* min)
{
    if (bufferTarget == sd::UniBufferTarget::VERTICES)
    {
        int num = kNumOfDataType[static_cast<int>(type)];
        int stride = kStrideOfAttributeType[static_cast<int>(attribute)];
        if (stride != num * sizeof(UniFloat))
        {
            std::cout << "Error: FillAccessorMaxMinValues attribute(" << static_cast<int>(attribute) << ") stride(" << stride << ") does not match!" << std::endl;
            return false;
        }
        int componentCount = sizeof(T) / sizeof(UniByte);
        for (int i = 0; i < num; i++)
        {
            float a = 0.0;
            float b = 0.0;
            for (int n = 0; n < count; n++)
            {
                const T* pos = (T*)data + stride/componentCount* n;
                float value = (float)pos[i];
                if (value > a) {
                    a = value;
                }

                if (value < b) {
                    b = value;
                }
            }

            max[i] = a;
            min[i] = b;
        }
    }
    return true;
}
    
extern std::unordered_map<int, int> kNumOfDataType;
extern std::unordered_map<int, int> kSizeOfComponentType;
extern std::unordered_map<int, int> kStrideOfAttributeType;

}

}