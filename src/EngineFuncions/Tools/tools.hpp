#define GLEW_STATIC
#ifndef TOOLS_HPP
#define TOOLS_HPP

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "../Handlers/ModelHandler/base_classes.h"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <iostream>

namespace tools
{

inline extern void check_mouse_reset(GLFWwindow* window,double xpos,double ypos,double swidth,double sheight,float border)
{
    if(xpos < border)
    {
        glfwSetCursorPos(window,border,ypos);
    }
    if(xpos > swidth-border)
    {
        glfwSetCursorPos(window,swidth-border,ypos);
    }
    if(ypos < border)
    {
        glfwSetCursorPos(window,xpos,border);
    }
    if(ypos > sheight-border)
    {
        glfwSetCursorPos(window,xpos,sheight-border);
    }
};

template<typename T>
inline extern void normal_device_coordinate(T &x,T &y,int swidth,int sheight)
{
    T xmid = swidth/(T)2.0;
    T ymid = sheight/(T)2.0;

    x = (x - xmid)/xmid;
    y = -(y - ymid)/ymid;
};

template<typename T>
inline extern void pixel_device_coordinate(T &x,T &y,int swidth,int sheight)
{
    T xmid = swidth/2.0f;
    T ymid = sheight/2.0f;

    x = xmid*(1+x);
    y = ymid*(1-y);
};

template<typename T>
inline extern void NDC_set_cursor_pos(GLFWwindow* window,T &x,T &y,int swidth,int sheight)
{
    T xpp=x,ypp=y;
    pixel_device_coordinate(xpp,ypp,swidth,sheight);
    glfwSetCursorPos(window,(double)xpp,(double)ypp);
};

//using namespace boost::filesystem;
inline extern std::vector<std::string> GetFilesInDirectory(std::string directory)
{
    boost::filesystem::path p(directory);
    std::vector<std::string> rtn;

    if(boost::filesystem::is_directory(p)) {
        std::cout << p << " is a directory containing:\n";

        for(auto& entry : boost::make_iterator_range(boost::filesystem::directory_iterator(p), {}))
        {
            //std::cout << entry << "\n";
            std::stringstream ss;
            ss << entry;
            rtn.push_back(ss.str());
        }
    }

    return rtn;
};

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
