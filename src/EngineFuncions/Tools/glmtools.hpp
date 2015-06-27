#ifndef GLMTOOLS_HPP
#define GLMTOOLS_HPP

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "../Handlers/ModelHandler/base_classes.h"
#include "rtscamera.h"
#include "tools.hpp"


#include <iostream>

namespace glmtools
{
    //************************************
    //   Determine Triangle Intersection
    //************************************
    /*
    */
    inline extern bool DetermineTriangleIntersection(double x,double z,double swidth, double sheight,glm::mat4 &VM,glm::mat4 &PM,glm::vec3 &orig,glm::vec3 &dir,glm::vec3 &v1,glm::vec3 &v2,glm::vec3 &v3,glm::vec3 &baryPos)
    {
        z=-1.0f*z; // Must flip the z axis to transform into 3D space properly.
        tools::pixel_device_coordinate(x,z,swidth,sheight);

        glm::vec3 rayfar=glm::unProject(glm::vec3((float)x,(float)z,1.0f),VM,PM,glm::vec4(0,0,swidth,sheight));
        glm::vec3 raynear=glm::unProject(glm::vec3((float)x,(float)z,0.0f),VM,PM,glm::vec4(0,0,swidth,sheight));

        glm::vec3 raypix=glm::normalize(rayfar-raynear);

        bool detisec = glm::intersectRayTriangle(orig,raypix,v1,v2,v3,baryPos);

        return detisec;
    };
}
#endif
