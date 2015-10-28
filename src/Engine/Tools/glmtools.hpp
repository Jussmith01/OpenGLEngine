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
    Simple GLM wrapper function for determining if a ray intersects a triangle.
    */
    inline extern bool DetermineTriangleIntersection(glm::vec3 &orig,glm::vec3 &raypix,glm::vec3 &v1,glm::vec3 &v2,glm::vec3 &v3,glm::vec3 &baryPos)
    {
        return glm::intersectRayTriangle(orig,raypix,v1,v2,v3,baryPos);
    };

    //************************************
    //   Determine Cube Intersection
    //************************************
    /*
    This function determines if a ray intersects a cube given the 8 corners of the cube.
    */
    inline extern bool DetermineSphereIntersection(glm::vec3 &orig,glm::vec3 &raypix,glm::vec3 &spherecent,double &width)
    {
        glm::vec3 pos;
        glm::vec3 norm;

        bool isec = glm::intersectRaySphere(orig,raypix,spherecent,width,pos,norm);

        return isec;
    };
}

#endif
