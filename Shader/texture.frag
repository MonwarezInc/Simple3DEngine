//GLSL version
#version 330
const int MAX_POINT_LIGHTS = 6;

//Input 

in vec2 coordTexture;
in vec3	Normal0;
in vec3	WorldPos0;

// Positional Light
struct BaseLight
{
	vec3	Color;
	float	AmbientIntensity;
	float	DiffuseIntensity;
};
struct DirectionalLight
{
	BaseLight	Base;
	vec3		Direction;
};
struct	Attenuation
{
	float	Constant;
	float	Linear;
	float	Exp;
};
struct	PointLight
{
	BaseLight	Base;
	vec3		Position;
	Attenuation	Atten;
};
uniform int 				NumPointLights;
uniform PointLight			PointLights[MAX_POINT_LIGHTS];			
uniform DirectionalLight 	directionalLight;
uniform vec3				eyeWorldPos;
uniform float				matSpecularIntensity;
uniform float				matSpecularPower;
// dont forget sampler2D text
uniform sampler2D 			text;
// Output
layout (location = 0) out vec4 out_Color; // for 330 and more

vec4	CalcLightInternal(BaseLight Light, vec3 LightDirection, vec3 Normal)
{
	
	// Ambient color
	vec4 	ambientColor	=	vec4(Light.Color,1.0f)*Light.AmbientIntensity;
	// diffuse factor
	float diffuseFactor		=	dot(Normal, -LightDirection);
	vec4 diffuseColor		=	vec4(0,0,0,0);
	vec4 specularColor		=	vec4(0,0,0,0);
	if (diffuseFactor > 0)
	{
		diffuseColor = vec4(Light.Color, 1.0f)* Light.DiffuseIntensity*diffuseFactor;
		vec3	VertexToEye		=	normalize(eyeWorldPos - WorldPos0);
		vec3	LightReflect	=	normalize(reflect(LightDirection, Normal));
		float	SpecularFactor	=	dot (VertexToEye, LightReflect);
		SpecularFactor			=	pow(SpecularFactor,matSpecularPower);
		if (SpecularFactor > 0)
		{
			specularColor	=	vec4(Light.Color,1.0f)*matSpecularIntensity*SpecularFactor;
		}
	}
	
	return (ambientColor + diffuseColor + specularColor);
}
vec4 	CalcDirectionalLight(vec3 Normal)
{
	return CalcLightInternal(directionalLight.Base, directionalLight.Direction, Normal);
}
vec4	CalcPointLight(int Index, vec3 Normal)
{
	vec3	LightDirection	=	WorldPos0 - PointLights[Index].Position;
	float	Distance		=	length(LightDirection);
	LightDirection			=	normalize(LightDirection);
	vec4	Color			=	CalcLightInternal(PointLights[Index].Base, LightDirection, Normal);
	float	atten			=	PointLights[Index].Atten.Constant +
								PointLights[Index].Atten.Linear * Distance +
								PointLights[Index].Atten.Exp * Distance * Distance;
	return	Color/atten;	
}
// Fonction main

void main()
{
	vec3 	Normal		=	normalize(Normal0);
	vec4	TotalLight	=	CalcDirectionalLight(Normal);
	for	(int i = 0; i < NumPointLights  && i < MAX_POINT_LIGHTS; ++i)
	{
		TotalLight +=	CalcPointLight(i, Normal);
	}
	out_Color	=	texture2D(text,coordTexture.xy)*TotalLight;
	
}

