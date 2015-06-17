#version 330 core
layout (location = 0) in vec2 position;

uniform float xpos;
uniform float ypos;

void main()
{
	float x = position.x + xpos;
	float y = position.y + ypos;
	gl_Position = vec4(x,y,0.0,1.0);
}

