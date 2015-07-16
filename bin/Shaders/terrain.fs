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
    // vec4 relative height stores:
    //x=highest point
    //y=LowtoMed switch
    //z=MetoHigh switch
    //w=+- switch blending
    vec4 relativeHeight;
    sampler2D lowlandMap;
    sampler2D mediumlandMap;
    sampler2D highlandMap;
    sampler2D cliffMap;
};
uniform Textures textures;

//*****************
//Skylight Function
//*****************
vec3 SkyLight(vec3 inputColor)
{
        //Properties
        vec3 outColor;

        vec3 lightDir = -normalize(skylightDir);
        vec3 viewDir = normalize(CameraPos - Position);
        vec3 reflectDir = reflect(-lightDir, Normal);

        //Shading
        float diffuse = max(dot(Normal, lightDir), 0.0) * skyIntensity;
        float specular = pow(max(dot(viewDir, reflectDir), 0.0), light.shininess) * skyIntensity;

        //Combining
        vec3 PointambientColor = light.ambient * inputColor;
        vec3 PointdiffuseColor = diffuse * light.diffuse * inputColor;
        vec3 PointspecularColor = specular * light.specular * inputColor;

        outColor = vec3(PointambientColor + PointdiffuseColor + PointspecularColor);

        return outColor;
}

//*****************
//Skylight Function
//*****************
vec4 HeightTexturing()
{
  // Must initialize return for += operator usage on
  // intel 4000 GPUs for sure, Dont know about other
  // intel or AMD yet. Nvidia Doesn't seem to have
  // this problem. In any case it is safer to
  // initialize manually.
  vec4 outColor=vec4(0.0);

  // Variables
  float x=textures.relativeHeight.x;
  float y=textures.relativeHeight.y;
  float z=textures.relativeHeight.z;
  float w=textures.relativeHeight.w;

  // Set Ranges
  float Range1=y-w;
  float Range2=y+w;
  float Range3=z-w;
  float Range4=z+w;

  // Normalized height
  float verty=(Position.y+x)/(x*2.0f);

  // Texture based on hight of verty
  if (verty <= Range1)
  {
    outColor = texture(textures.lowlandMap,TexCoord);
  }
  else if (verty <= Range2)
  {
    float transition = smoothstep(Range1,Range2,verty);
    outColor+=texture(textures.lowlandMap, TexCoord) * (1.0-transition);
    outColor+=texture(textures.mediumlandMap, TexCoord) * transition;
  }
  else if (verty <= Range3)
  {
    outColor = texture(textures.mediumlandMap,TexCoord);
  }
  else if (verty <= Range4)
  {
    float transition = smoothstep(Range3,Range4,verty);
    outColor+=texture(textures.mediumlandMap,TexCoord) * (1.0-transition);
    outColor+=texture(textures.highlandMap,TexCoord) * transition;
  }
  else
  {
    outColor = texture(textures.highlandMap,TexCoord);
  }

  float dNorm=dot(vec3(0.0f,1.0f,0.0f),Normal);
  if(dNorm<=0.85)
  {
    outColor = texture(textures.cliffMap,TexCoord);
  }
  else if (dNorm<=0.9)
  {
    float transition = smoothstep(0.85,0.9,dNorm);
    outColor = mix(texture(textures.cliffMap,TexCoord),outColor,transition);
  }

  return outColor;
}

//*****************
//	Main
//*****************
out vec4 color;
void main()
{
    //Apply the height map textures
	vec4 outColor=HeightTexturing();

	//Apply Directional Sky Lighting
	outColor=vec4(SkyLight(vec3(outColor)),outColor.w);

	color=outColor;
	//color = vec4(0.5f,0.0f,0.5f,1.0f);
}

