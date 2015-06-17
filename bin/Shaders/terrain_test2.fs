#version 330 core

in vec3 CameraPos;
in vec3 Normal;

layout (std140) uniform globallightData
{
        vec3 lightDir; //Direction
	vec3 lightInt; //Intensity
};

struct Material {
    sampler2D diffuseMap;
};

vec3 DirectionalLight(vec3 Position,vec3 norm,Material material,Light light,DirectionalLightVars Dlight,vec3 cPos)
{
        //Properties
        vec3 output;

        vec3 lightDir = -normalize(Dlight.direction);
        vec3 viewDir = normalize(cPos - Position);
        vec3 reflectDir = reflect(-lightDir, norm);
        
        //Shading
        float diffuse = max(dot(norm, lightDir), 0.0) * Dlight.intensity;
        float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess) * Dlight.intensity;
        
        //Combining
        vec3 PointambientColor = light.ambient * vec3(texture(material.diffuseMap,TexCoord));
        vec3 PointdiffuseColor = diffuse * light.diffuse * vec3(texture(material.diffuseMap,TexCoord)); //* light.intensity;

        vec3 greyscaleMap = vec3(texture(material.specularMap,TexCoord));
        float specIntensity = (greyscaleMap.x + greyscaleMap.y + greyscaleMap.z)/3.0f;
        vec3 PointspecularColor = specular * light.specular * specIntensity * vec3(texture(material.diffuseMap,TexCoord));  //* light.intensity;
 
        output = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);

        return output;
};

out vec4 color;
uniform Material material;

void main()
{
	//color = vec4(CameraPos,1.0f);
	color = vec4(0.5f,0.0f,0.5f,1.0f);
}

