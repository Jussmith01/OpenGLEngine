#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform float xpos;
uniform float ypos;

void main()
{
	float x = position.x + xpos;
	float y = position.y + ypos;
	gl_Position = vec4(x,y,0.0,1.0);
	// We swap the y-axis by substracing our coordinates from 1. This is done because most images have the top y-axis inversed with OpenGL's top y-axis.
	// TexCoord = texCoord;
	TexCoord = vec2(texCoord.x, 1.0 - texCoord.y);
}

