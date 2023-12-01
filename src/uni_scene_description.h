#pragma once 
#include <stddef.h>
#include <string>
#include <vector>

namespace universe 
{
typedef struct UniNode {
    char *name;
    UniNode* parent;
    std::vector<UniNode*> children;
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
};

}