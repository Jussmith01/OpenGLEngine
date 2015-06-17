#version 330 core
out vec4 color;

// Color
uniform vec4 incolor;

void main()
{
	color = incolor;
	//color = vec4(1.0);
}

