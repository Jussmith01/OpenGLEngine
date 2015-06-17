#include "model.h"


Model::Model(std::string objfile)
{
    this->objfile = objfile;
    CPULoad = false;
    GPULoad = false;
    INSTLoad = false;
};

void Model::LoadModelFileToCPU()
{
    if (!CPULoad)
    {
        CPULoad=true;
        objLoader f(objfile.c_str());
        Nmesh = f.RtnNumMesh();

        for (int i = 0; i < Nmesh; ++i)
        {
            mesh.push_back(f.CreateMesh(i,mTBN[i]));
            //mesh[i].SetMeshOnDevice();
        }
        ModelPosition = glm::vec3(0.0f,0.0f,-1.0f);
    }
};

void Model::LoadModelDataToCPU()
{
    if (!CPULoad)
    {
        CPULoad=true;
        objLoader f(objfile.c_str());
        Nmesh = f.RtnNumMesh();

        for (int i = 0; i < Nmesh; ++i)
        {
            mesh.push_back(f.CreateMesh(i,mTBN[i]));
            //mesh[i].SetMeshOnDevice();
        }
        ModelPosition = glm::vec3(0.0f,0.0f,-1.0f);
    }
};

void Model::LoadModelToGPU()
{
    if (CPULoad)
    {
        if (!GPULoad)
        {
            GPULoad=true;
            //cout << "loading Model to GPU: MESHSIZE: " << mesh.size() << "\n";
            for (int i = 0; i < Nmesh; ++i)
            {
                mesh[i].SetMeshOnDevice();
            }
        }
    }
    else
    {
        std::cout << "ERROR: Data not loaded to CPU!\n";
    }
};

void Model::ClearMeshes()
{
    if (CPULoad)
    {
        if (GPULoad)
        {
            CPULoad=true;
            GPULoad=false;
            //cout << "loading Model to GPU: MESHSIZE: " << mesh.size() << "\n";
            for (int i = 0; i < Nmesh; ++i)
            {
                mesh[i].Cleanup();
            }
        }
        else
        {
            std::cout << "ERROR: Data not loaded to GPU!\n";
        }
    }
    else
    {
        std::cout << "ERROR: Data not loaded to CPU!\n";
    }
};

void Model::SetupInstancing(std::vector<glm::mat4> &modelMatrices)
{
    if (CPULoad)
    {
        if (GPULoad)
        {
            //cout << "loading Model to GPU: MESHSIZE: " << mesh.size() << "\n";
            INSTLoad = true;
            for (int i = 0; i < Nmesh; ++i)
            {
                mesh[i].setupInstanceArray(modelMatrices);
            }
        }
        else
        {
            std::cout << "ERROR: Data not loaded to GPU!\n";
        }
    }
    else
    {
        std::cout << "ERROR: Data not loaded to CPU!\n";
    }
};

void Model::UpdateInstancedModelMatrices(std::vector<glm::mat4> &modelMatrices)
{
    if (CPULoad)
    {
        if (GPULoad)
        {
            if (INSTLoad)
            {
                for (int i = 0; i < Nmesh; ++i)
                {
                    mesh[i].UpdateInstancedModelMatrices(modelMatrices);
                }
            }
            else
            {
                std::cout << "ERROR: Instancing is not setup for this model!\n";
            }
        }
        else
        {
            std::cout << "ERROR: Data not loaded to GPU!\n";
        }
    }
    else
    {
        std::cout << "ERROR: Data not loaded to CPU!\n";
    }
};
