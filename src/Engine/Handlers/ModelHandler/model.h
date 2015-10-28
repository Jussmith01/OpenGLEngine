#ifndef MODELHANDLER_W
#define MODELHANDLER_W

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "mesh.h"
#include "../../Loaders/meshloader.h"

class Model
{
public:
    //Declare a vector for our mesh classes
    glm::vec3 ModelPosition;
    std::vector<Mesh> mesh;
    std::vector<bool> mTBN; //Calculates the meshes tangent and bitangent vectors
    std::string objfile;
    int Nmesh;
    bool CPULoad;
    bool GPULoad;
    bool INSTLoad;

    Model(std::string objfile);

    void LoadModelFileToCPU();

    void LoadModelDataToCPU();

    void LoadModelToGPU();

    void ClearMeshes();

    void SetupInstancing(std::vector<glm::mat4> &modelMatrices);

    void UpdateInstancedModelMatrices(std::vector<glm::mat4> &modelMatrices);

    ~Model() {};
};

#endif
