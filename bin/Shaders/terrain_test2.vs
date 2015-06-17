#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

layout (std140) uniform cameraData
{
        vec3 camPos;
        mat4 projMat;
        mat4 viewMat;
};

out vec3 CameraPos;

uniform mat4 modelMat;

void main()
{
	gl_Position = projMat * viewMat * modelMat * vec4(position, 1.0f);
	//gl_Position = viewMat * modelMat * vec4(position, 1.0f);
	//gl_Position = projMat * viewMat * vec4(position, 1.0f);
	CameraPos = camPos;
}

