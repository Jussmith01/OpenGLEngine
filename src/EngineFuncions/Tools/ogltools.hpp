#ifndef OGLTOOLS_HPP
#define OGLTOOLS_HPP

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
//#include "../Handlers/ModelHandler/base_classes.h"

#include <iostream>

namespace ogltools
{

struct BufferHandler
{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    int idxSize;

    BufferHandler () {};

    //Class Assignment
    BufferHandler& operator=(const BufferHandler& instance)
    {
        this->VAO = instance.VAO;
        this->VBO = instance.VBO;
        this->EBO = instance.EBO;
        this->idxSize=instance.idxSize;
        return *this;
    }

    void GenBuffers(std::vector<Vertex> &verts,std::vector<GLuint> &idxs)
    {
        idxSize=(int)idxs.size();

        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(Vertex), &verts[0], GL_DYNAMIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, idxs.size() * sizeof(GLuint), &idxs[0], GL_DYNAMIC_DRAW);

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

        glBindVertexArray(0);
    };

    void DrawVerts()
    {
        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, idxSize, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    };

    void ClearBuffers()
    {
        idxSize=0;
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    };
};

}
#endif
