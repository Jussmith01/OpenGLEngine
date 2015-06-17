#include "mesh.h"

void Mesh::SetMeshOnDevice()
{
    if (!TBN)
    {
        setupMeshRegular();
    }
    else
    {
        setupMeshWithTangents();
    }
};

// Render the mesh
//*********************************************
//         Draws the Mesh (Singular)
//*********************************************
void Mesh::Draw()
{
    // Draw mesh
    //cout << "Drawing Mesh: " << mID << endl;
    glBindVertexArray(this->VAO);
    //cout << "TEST1" << endl;
    glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    //cout << "TEST2" << endl;
    glBindVertexArray(0);
    //cout << "TEST3" << endl;
};

// Render the mesh
//*********************************************
//         Draws the Mesh (Instanced)
//*********************************************
void Mesh::DrawInstanced(int NumberInstanced)
{
    // Draw mesh
    //cout << "Drawing " << NumberInstanced << " Instanced Meshes: " << mID << endl;
    glBindVertexArray(this->VAO);
    //cout << "TEST1" << endl;
    //glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
    glDrawElementsInstanced(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0, NumberInstanced);
    //cout << "TEST2" << endl;
    glBindVertexArray(0);
    //cout << "TEST3" << endl;
};

//*********************************************
//                   Cleanup
//*********************************************
/*
Frees the entire class
*/
void Mesh::Cleanup()
{
    CleanupGPU();
    CleanupCPU();
};

//*********************************************
//              Cleanup GPU
//*********************************************
/*
Frees buffers allocated on the GPU
*/
void Mesh::CleanupGPU()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    if (TBN)
    {
        glDeleteBuffers(1, &IPBO);
    }
};

//*********************************************
//              Cleanup GPU
//*********************************************
/*
Frees vectors allocated on the CPU
*/
void Mesh::CleanupCPU()
{
    vertices.clear();
    indices.clear();

    if (TBN)
    {
        verticeswtang.clear();
    }
};

//*********************************************
//            Setup a Regular Mesh
//*********************************************
void Mesh::setupMeshRegular()
{
    std::cout << "Setting Up Regular Mesh" << std::endl;
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Vertex Tex Coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, texture));
    // Vertex Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, normal));

    VAOidx = 2;

    glBindVertexArray(0);
};

//*********************************************
//    Setup Mesh with Calculated Tangents
//*********************************************
void Mesh::setupMeshWithTangents()
{
    std::cout << "Setting Up Mesh With Tangents" << std::endl;
    // Create buffers/arrays
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    glBindVertexArray(this->VAO);
    // Load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, this->verticeswtang.size() * sizeof(VertexwTang), &this->verticeswtang[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

    // Set the vertex attribute pointers
    // Vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)0);
    // Vertex Tex Coords
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, texture));
    // Vertex Normals
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, normal));
    // Vertex Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, tangent));
    // Vertex BiTangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexwTang), (GLvoid*)offsetof(VertexwTang, bitangent));

    VAOidx = 4;

    glBindVertexArray(0);
};

//*********************************************
//	Add an instancing VBO to the VAO
//*********************************************
void Mesh::setupInstanceArray(std::vector<glm::mat4> &modelMatrices)
{
    std::cout << "Setting Up Instancing VBO\n";
    int amount = modelMatrices.size();

    // Vertex Buffer Object
    glBindVertexArray(VAO);
    glGenBuffers(1, &IPBO);
    std::cout << "Amount: " << amount << " VAO/IPBO: " << VAO << "/" << IPBO << std::endl;
    std::cout << "ModMatPos: " << modelMatrices[0][3][0] << "," << modelMatrices[0][3][1] << "," << modelMatrices[0][3][2] << std::endl;
    glBindBuffer(GL_ARRAY_BUFFER, IPBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), &modelMatrices[0], GL_STREAM_DRAW);
    // Vertex Attributes
    glEnableVertexAttribArray(VAOidx+1);
    glVertexAttribPointer(VAOidx+1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)0);
    glEnableVertexAttribArray(VAOidx+2);
    glVertexAttribPointer(VAOidx+2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(sizeof(glm::vec4)));
    glEnableVertexAttribArray(VAOidx+3);
    glVertexAttribPointer(VAOidx+3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(2 * sizeof(glm::vec4)));
    glEnableVertexAttribArray(VAOidx+4);
    glVertexAttribPointer(VAOidx+4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (GLvoid*)(3 * sizeof(glm::vec4)));

    glVertexAttribDivisor(VAOidx+1, 1);
    glVertexAttribDivisor(VAOidx+2, 1);
    glVertexAttribDivisor(VAOidx+3, 1);
    glVertexAttribDivisor(VAOidx+4, 1);
    //ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    glBindVertexArray(0);
};

//*********************************************
//      Add an instancing VBO to the VAO
//*********************************************
void Mesh::UpdateInstancedModelMatrices(std::vector<glm::mat4> &modelMatrices)
{
    // Now copy data into memory
    int amount = modelMatrices.size();
    //long int MemSize = amount * sizeof(glm::mat4);

    //if (MemSize > 1.0E6)
    //{
    // 	glBindBuffer(GL_ARRAY_BUFFER, IPBO);
    //	glBufferSubData(IPBO,0,MemSize,&modelMatrices[0]);
    //} else {

    //GLenum ErrorChk1 = glGetError();
    //double timeStartMain = glfwGetTime();
    //int amount = modelMatrices.size();

    glBindBuffer(GL_ARRAY_BUFFER, IPBO);
    // Get pointer
    GLfloat* NullPtr = NULL;
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::mat4), NullPtr, GL_STREAM_DRAW);
    //double timeMapBuffer = glfwGetTime();

    GLenum ErrorChk2 = glGetError();
    if (ErrorChk2 != GL_NO_ERROR)
    {
        //std::cout << "!!GL Error " << ErrorChk2 << " Detected!! :: " << " :: In ErrorPos1 " << "\n";
        std::cout << "!!GL Error " << ErrorChk2 << " Detected!! :: " << gluErrorString(ErrorChk2) << " :: In ErrorPos1 " << "\n";
    }


    GLvoid* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    GLenum ErrorChk3 = glGetError();
    if (ErrorChk3 != GL_NO_ERROR)
    {
        //std::cout << "!!GL Error " << ErrorChk3 << " Detected!! :: " << " :: In ErrorPos2 " << "\n";
        std::cout << "!!GL Error " << ErrorChk3 << " Detected!! :: " << gluErrorString(ErrorChk3) << " :: In ErrorPos2 " << "\n";
    }

    //timeMapBuffer = (glfwGetTime() - timeMapBuffer)*1000.0;

    if (ptr == NULL)
    {
        std::cout << "ERROR::MAPPEDBUFFERPOINTER::Problem Mapping Buffer to Pointer" << std::endl;
    }

    // Now copy data into memory
    //double timeStart = glfwGetTime();

    try
    {
        memcpy(ptr, &modelMatrices[0], amount * sizeof(glm::mat4));
    }
    catch ( std::exception e )
    {
        std::cout << "ERROR::INSTANCEDMEMCPY::Problem Loading Data" << std::endl;
    }

    //double MemCpyTime = glfwGetTime() - timeStart;
    //double MemCpySpeed = (double)MemSize / (MemCpyTime);

    // Make sure to tell OpenGL we're done with the pointer
    if(!glUnmapBuffer(GL_ARRAY_BUFFER))
    {
        std::cout << "!!!Error: Unable to unmap VBO!!!\n";
    };

    //cout << "Instancing Memcopy Speed: " << MemCpySpeed / (double)(1024*1024*1024) << " GB/s - Total Time(ms): " << (glfwGetTime() - timeStartMain) * 1000.0 << " MemCpySize(MB): " << MemSize/(1024.0*1024.0) << " MemCpyTime(ms): " << MemCpyTime*1000 << " BufferMapTime(ms): " << timeMapBuffer << endl;
    //}
};

//*********************************************
//      	Set Materials
//*********************************************
void Mesh::setMaterial (GLint Prog)
{
    GLint matAmbientLoc = glGetUniformLocation(Prog, "light.ambient");
    GLint matDiffuseLoc = glGetUniformLocation(Prog, "light.diffuse");
    GLint matSpecularLoc = glGetUniformLocation(Prog, "light.specular");
    GLint matShineLoc = glGetUniformLocation(Prog, "light.shininess");

    //cout << "Ambient [" << materials.Ka.x << "," << materials.Ka.y << "," << materials.Ka.z << "]\n";

    glUniform3f(matAmbientLoc, materials.Ka.x, materials.Ka.y, materials.Ka.z);
    glUniform3f(matDiffuseLoc, materials.Kd.x, materials.Kd.y, materials.Kd.z);
    glUniform3f(matSpecularLoc, materials.Ks.x, materials.Ks.y, materials.Ks.z);
    glUniform1f(matShineLoc, materials.shine);
};
