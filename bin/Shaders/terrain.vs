#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 normal;

layout (std140) uniform cameraData
{
        vec3 camPos;
        mat4 projMat;
        mat4 viewMat;
};

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec3 CameraPos;

uniform mat4 modelMat;

void main()
{
	gl_Position=projMat * viewMat * modelMat * vec4(position, 1.0f);
	
	// Variables passed to the frangment shader
	Position=vec3(modelMat * vec4(position, 1.0f));
	Normal=normal;
	TexCoord=vec2(texture.x,1.0f-texture.y);
	CameraPos=camPos;
}

