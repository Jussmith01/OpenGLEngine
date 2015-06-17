#ifndef RTSCAMERA_C
#define RTSCAMERA_C

#include "../../Headers/headerscpp.h"
#include "../../Headers/headersogl.h"
#include "input_struct.h"
#include "../Loaders/shader.h"


//************************************
//    Real-time Strategy Camera
//************************************
/*

*/
class RTSCamera
{
    //*******************************
    // Verticle Pole Positional Data
    //*******************************
    /*cameraData
    vPole is vertical on the y axis, games space
    is in the x,z axis space.
    */
    glm::vec3 vPole; // Should be (0.0,height of cam,0.0)
    glm::vec3 vPolePos; // Position from the origin

    //*********************************
    // Horizontal Pole Positional Data
    //*********************************
    /*
    hPole is the horizontal pole pointing from
    the top of the vertical pole to the camera.
    */
    glm::vec3 hPole;

    // Basic Variables
    GLfloat cameraMovementSpeed;
    GLfloat cameraRotationSpeed;
    GLfloat cameravHeightSpeed;
    GLfloat camerahLengthSpeed;
    double ft; // Frame Time
    double dt; // Delta time
    float aspect;
    int swidth;
    int sheight;

    //____________________________________
    //       CAMERAUBO Variable
    //------------------------------------

    GLuint cameraDataUBO;
    /* !!!!THIS UBO IS ALWAYS BOUND TO

        GL_UNIFORM_BUFFER index 0
    */

public:
    //*************************
    // Camera Positional Data
    //*************************
    glm::vec3 cameraPos;

    //************************
    // Camera Direction Data
    //************************
    glm::vec3 cameraDir;

    //**************************
    // Transformation Matricies
    //**************************
    glm::mat4 PM;
    glm::mat4 VM;
    glm::vec3 Cursor3DPos;

    //**************
    //  Constructor
    //**************
    RTSCamera () {};

    //*********************************
    //     RTS Camera Initializer
    //*********************************
    /*
    Initialize the camera with:
        1) Height
        2) Dist from center
        3) x Position of the center
        4) y Position of the center
    Initializes the cam speeds at a default value.
    Sets the Camera variables using the input data
    and the SetCamera() function.
    */
    void Init(float camheight,float camhdist,float centX, float centZ, int swidth, int sheight)
    {
        // Initialize Poles
        vPole=glm::vec3(0.0f,camheight,0.0f);
        vPolePos=glm::vec3(centX,0.0f,centZ);
        hPole=glm::vec3(0.0f,0.0f,camhdist);
        cameraMovementSpeed=0.1f;
        cameraRotationSpeed=1.0f;
        cameravHeightSpeed=0.1f;
        camerahLengthSpeed=0.1f;

        this->aspect=(float)swidth/(float)sheight;
        this->swidth=swidth;
        this->sheight=sheight;

        ft=glfwGetTime();
        dt=0.0;

        SetCamera();
        SetCameraDataUBO();
    };

    //*********************************
    //     Set the Camera Speeds
    //*********************************
    /*
    Input:
        1) The desired cam movement speed
        2) The desired cam rotation speed

    */
    void SetCameraSpeeds(float camMov,float camRot, float camHei, float camLei)
    {
        this->cameraMovementSpeed=camMov;
        this->cameraRotationSpeed=camRot;
        this->cameravHeightSpeed=camHei;
        this->camerahLengthSpeed=camLei;
    };

    //*********************************
    //       Update the Camera
    //*********************************
    /*
    Input:
        1) inputstruct from the state class

    Run this during the event update of the state cycle.
    The camera data is loaded to the cameraData Uniform
    Buffer Object (UBO) for use by any shader when is
    registered with this class and contains the cameraData
    UBO.
    */
    void UpdateCamera(InputStruct &input)
    {
        Setdt();
        MoveCameraVPole(input);
        RotateCameraHPole(input);
        ChangeLengthCameraVPole(input);
        ChangeLengthCameraHPole(input);
        UpdateCursor3DPos(input);
        SetCamera();
        UpdateCameraDataUBO();
    };

    //*********************************
    //      Cleanup the Class
    //*********************************
    /*
    The only cleanup required is the deletion of
    the cameraData UBO from the GPU.
    */
    void Cleanup()
    {
        glDeleteBuffers(1,&cameraDataUBO);
    };

    //*********************************
    //   Register Shader for UBO
    //*********************************
    /*
    This functions allows you to register a shader
    with the class so that it can obtain the UBO
    Data. All shaders that will use this UBO MUST
    register with this class!
    */
    void RegisterShaderWithCameraDataUBO(Shader &shader)
    {
        //do this for all active shaders
        GLuint P = shader.Program;
        GLuint uniformBlockIndex = glGetUniformBlockIndex(P, "cameraData");
        glUniformBlockBinding(P, uniformBlockIndex, 0);
    };

    glm::vec3 GetvPolPos()
    {
        return vPolePos;
    }
private:

    //*********************************
    //  Move the Cameras Vertical Pole
    //*********************************
    /*
    Input:
        1) inputstruct from the state class

    This moves the vertical pole of the camera
    based on W,A,S,D input.
    w=forward; s=backward
    a=pan left; d=pan right
    */
    void MoveCameraVPole (InputStruct &input)
    {
        if(input.GetKey(GLFW_KEY_W))
        {
            vPolePos -= cameraMovementSpeed * hPole * 0.2f;
            //std::cout << "W x: " << vPolePos.x << " y: " << vPolePos.y << " z: " << vPolePos.z << std::endl;
            //std::cout << "CP x: " << cameraPos.x << " y: " << cameraPos.y << " z: " << cameraPos.z << std::endl;
            //std::cout << "CD x: " << cameraDir.x << " y: " << cameraDir.y << " z: " << cameraDir.z << std::endl;
        }
        if(input.GetKey(GLFW_KEY_S))
        {
            vPolePos += cameraMovementSpeed * hPole * 0.2f;
            //std::cout << "S x: " << vPolePos.x << " y: " << vPolePos.y << " z: " << vPolePos.z << std::endl;
        }
        if(input.GetKey(GLFW_KEY_A))
        {
            vPolePos += glm::normalize(glm::cross(hPole, vPole)) * cameraMovementSpeed;
            //std::cout << " A x: " << vPolePos.x << " y: " << vPolePos.y << " z: " << vPolePos.z << std::endl;
        }
        if(input.GetKey(GLFW_KEY_D))
        {
            vPolePos -=  glm::normalize(glm::cross(hPole, vPole)) * cameraMovementSpeed;
            //std::cout << "D x: " << vPolePos.x << " y: " << vPolePos.y << " z: " << vPolePos.z << std::endl;
        }
    };

    //************************************
    // Rotate the Cameras Horizontal Pole
    //************************************
    /*
    Input:
        1) inputstruct from the state class

    This rotates the horizontal pole of the
    camera based on Q,E input.

    Q = rotate the camera to the right
    E = rotate the camera to the left
    */
    void RotateCameraHPole(InputStruct &input)
    {
        if(input.GetKey(GLFW_KEY_Q))
        {
            float angle=-cameraRotationSpeed*(float)dt;
            hPole=glm::rotate(hPole,angle,vPole);
        }

        if(input.GetKey(GLFW_KEY_E))
        {
            float angle=cameraRotationSpeed*(float)dt;
            hPole=glm::rotate(hPole,angle,vPole);
        }
    };

    //***********************************************
    // Change Length of the Cameras Horizontal Pole
    //***********************************************
    /*
    Input:
        1) inputstruct from the state class

    This changes the length of the horizontal
    pole of the camera based on R,F input.

    R = Make the hPole length shorter
    F = Make the hPole length longer
    */
    void ChangeLengthCameraHPole(InputStruct &input)
    {
        if(input.GetKey(GLFW_KEY_R))
        {
            glm::vec3 hPoleNormal = glm::normalize(hPole);
            hPole-=hPoleNormal*(float)dt*camerahLengthSpeed;
        }

        if(input.GetKey(GLFW_KEY_F))
        {
            glm::vec3 hPoleNormal = glm::normalize(hPole);
            hPole+=hPoleNormal*(float)dt*camerahLengthSpeed;
        }
    };

    //************************************
    // Change Camera Height with Scroll
    //************************************
    /*
    Input:
        1) inputstruct from the state class

    This changes the height of the cameras vPole
    by scrolling the mouse wheel. This is sort of a
    zoom feature.:q
    */
    void ChangeLengthCameraVPole(InputStruct &input)
    {
        vPole=vPole-glm::normalize(vPole)*(float)input.sy*cameravHeightSpeed;
        if (vPole.y < 0.5f)
        {
            vPole.y=0.5f;
        }
        input.sy=0.0;
    };

    //************************************
    //      Update Cursor 3D Position
    //************************************
    /*
    Input:
        1) inputstruct from the state class

    This calculates the cursors position in 3D
    space on the x,z plane specifically.

    This is accomplished by obtaining a vector
    from the camera to the near plane of the
    perspective matrix transform (raynear) and
    one to the far plane (rayfar). The vector
    between these two points is the real vector
    between the points in 3D space. Normalizing
    the difference give the unit vector of this
    ray. Then, obtaining the distance to the
    intersection of this ray/unit vector and the
    x,z plane allows calculation of a vector to
    that point in 3D space from the camera. This
    vector along with the camera position vector
    is used to calculate the 3D space position of
    the mouse on the x,y plane.
    */
    void UpdateCursor3DPos(InputStruct &input)
    {
        double x,z;
        input.ReturnMousePos(x,z);
        z=-1.0f*z; // Must flip the z axis to transform into 3D space properly.
        tools::pixel_device_coordinate(x,z,swidth,sheight);

        glm::vec3 rayfar=glm::unProject(glm::vec3((float)x,(float)z,1.0f),VM,PM,glm::vec4(0,0,swidth,sheight));
        glm::vec3 raynear=glm::unProject(glm::vec3((float)x,(float)z,0.0f),VM,PM,glm::vec4(0,0,swidth,sheight));

        glm::vec3 raypix=glm::normalize(rayfar-raynear);

        float distance=0;
        glm::intersectRayPlane(cameraPos,raypix,glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f),distance);
        Cursor3DPos=cameraPos+raypix*(distance);
    };

    //************************************
    //    Set the Cameras Pos and Dir
    //************************************
    /*
    Sets the cameras Position and Direction based
    on how the input changed the cameras definition
    vectors(vPole,hPole,vPolPos).
    */
    void SetCamera()
    {
        SetCameraPos();
        SetCameraDir();
    };

    //************************************
    //     Set the Cameras Position
    //************************************
    /*
    Set the cameras position based on new data.
    */
    void SetCameraPos()
    {
        cameraPos=vPole+hPole+vPolePos;
    };

    //************************************
    //      Set the Camera Direction
    //************************************
    /*
    Set the camera direction base on new data.
    */
    void SetCameraDir()
    {
        cameraDir=-glm::normalize(hPole+vPole);
    };

    //************************************
    //    Set the time between frames
    //************************************
    /*
    obtain the time between the frames.
    */
    void Setdt()
    {
        dt=glfwGetTime()-ft;
        ft=glfwGetTime();
    };

    //************************************
    //    Get Frame Time (In ms)
    //************************************
    /*

    */
    float GetFrameTimems()
    {
        return dt/(float)1000.0;
    };

    //************************************
    // Initialize the cameraData UBO
    //************************************
    /*
    This class sets the cameraData UBO so that
    shaders can access the new data on the GPU.
    */
    void SetCameraDataUBO()
    {
        glGenBuffers(1, &cameraDataUBO);

        glBindBuffer(GL_UNIFORM_BUFFER, cameraDataUBO);
        glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 16, NULL, GL_DYNAMIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        glBindBufferRange(GL_UNIFORM_BUFFER, 0, cameraDataUBO, 0, 2 * sizeof(glm::mat4) + sizeof(glm::vec3));
    };

    //************************************
    //    Update the cameraData UBO
    //************************************
    /*
    Updates the data in the previously set
    cameraData UBO.

    A shader, with the cameraData UBO must
    register with the

    RegisterShaderWithCameraDataUBO(Shader &shader)

    function of this class to obtain the
    data from this UBO.
    */
    void UpdateCameraDataUBO ()
    {
        // Setup Space Tranformation Matricies
        PM=glm::perspective(glm::radians(45.0f),aspect,10.0f,10000.0f);
        VM=glm::lookAt(cameraPos, cameraPos + cameraDir, glm::vec3(0.0f,1.0f,0.0f));

        // Bind the UBO Buffer
        glBindBuffer(GL_UNIFORM_BUFFER, cameraDataUBO);

        // Set the Projection Matrix in the UBO
        glBufferSubData(GL_UNIFORM_BUFFER, 0, 16, glm::value_ptr(cameraPos));

        // Set the Projection Matrix in the UBO
        glBufferSubData(GL_UNIFORM_BUFFER, 16, sizeof(glm::mat4), glm::value_ptr(PM));

        // Set the View Matrix in the UBO
        glBufferSubData(GL_UNIFORM_BUFFER, 16 + sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(VM));

        // Unbind the UBO Buffer
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    };
};

#endif
