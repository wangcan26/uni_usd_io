#pragma once

namespace universe 
{

typedef struct UniSettings
{
    float scale;
    bool is_sequence;
    int  sequence_len;
    bool validate_meshes;
    char mesh_read_flag;
    bool import_meshes;
}UniSettings;

}