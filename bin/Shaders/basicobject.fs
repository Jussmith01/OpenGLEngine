#version 330 core

in vec3 CameraPos;
in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

//*******************
// Skylight Data UBO
//*******************
layout (std140) uniform skylightData
{
	float skyIntensity;
	vec3 skylightColor;
        vec3 skylightDir;
};

//*****************
//  Light Struct
//*****************
struct Light {
    float shininess;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;

//*****************
// Textures Struct
//*****************
struct Textures {
    sampler2D textureMap;
};
uniform Textures textures;

//*****************
//Skylight Function
//*****************
vec3 SkyLight(vec3 input)
{
        //Properties
        vec3 output;

        vec3 lightDir = -normalize(skylightDir);
        vec3 viewDir = normalize(CameraPos - Position);
        vec3 reflectDir = reflect(-lightDir, Normal);
        
        //Shading
        float diffuse = max(dot(Normal, lightDir), 0.0) * skyIntensity;
        float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess) * skyIntensity;
        
        //Combining
        vec3 PointambientColor = light.ambient * input;
        vec3 PointdiffuseColor = diffuse * light.diffuse * input;
	vec3 PointspecularColor = specular * light.specular * input;
 
        output = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);

        return output;
};

//*****************
//	Main
//*****************
out vec4 color;
void main()
{
	vec3 output = vec3(texture(textures.textureMap, TexCoord));
	
	//Apply Directional Sky Lighting
	output=SkyLight(output);
	
	color=vec4(vec3(output),1.0f);
	//color = vec4(0.5f,0.0f,0.5f,1.0f);
}

