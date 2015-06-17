#ifndef MESHDATA_S
#define MESHDATA_S

#include "../../../Headers/headerscpp.h"
#include "../../../Headers/headersogl.h"

struct Vertex
{
	//Input from .obj file
        glm::vec3 position;//Stores vertex positions
        glm::vec2 texture;//Stores vertex positions
        glm::vec3 normal;//Stores vertex positions
};

struct VertexwTang
{
        //Input from .obj file
        glm::vec3 position;//Stores vertex positions
        glm::vec2 texture;//Stores vertex positions
        glm::vec3 normal;//Stores vertex positions

        //Calculated if a displacement or normal map
        //is included with the model
        glm::vec3 tangent;//Stores vertex positions
        glm::vec3 bitangent;//Stores vertex positions
};


struct Material
{
        GLfloat shine;// Shine factor
        glm::vec3 Ka;//Ambient lighting vector
        glm::vec3 Kd;//Diffuse lighting vector
        glm::vec3 Ks;//Specular lighting vector
        std::string texfilename;//Texture filename
};
#endif
