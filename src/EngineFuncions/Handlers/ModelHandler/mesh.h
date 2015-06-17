#ifndef MESHHANDLER_W
#define MESHHANDLER_W

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"
#include "base_classes.h"

//**********************************
//Begin Mesh class -- Creates meshes
//**********************************
class Mesh
{
public:
    int mID;
    std::vector<Vertex> vertices;
    std::vector<VertexwTang> verticeswtang;
    std::vector<GLuint> indices;
    Material materials;
    bool TBN;//Bool for tangent vectors

    //Constructor
    Mesh(int mID)
    {
        this->mID=mID;
    };

    //Destructor
    ~Mesh() {};

    void SetMeshOnDevice();

    // Render the mesh
    //*********************************************
    //         Draws the Mesh (Singular)
    //*********************************************
    void Draw();

    // Render the mesh
    //*********************************************
    //         Draws the Mesh (Instanced)
    //*********************************************
    void DrawInstanced(int NumberInstanced);

    void Cleanup();

    void CleanupGPU();

    void CleanupCPU();

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO, IPBO;
    GLuint VAOidx;
    void* ptr;

    //*********************************************
    //            Setup a Regular Mesh
    //*********************************************
    void setupMeshRegular();

    //*********************************************
    //    Setup Mesh with Calculated Tangents
    //*********************************************
    void setupMeshWithTangents();

public:
    //*********************************************
    //	Add an instancing VBO to the VAO
    //*********************************************
    void setupInstanceArray(std::vector<glm::mat4> &modelMatrices);

    //*********************************************
    //      Add an instancing VBO to the VAO
    //*********************************************
    void UpdateInstancedModelMatrices(std::vector<glm::mat4> &modelMatrices);

    //*********************************************
    //      	Set Materials
    //*********************************************
    void setMaterial (GLint Prog);
};

#endif
