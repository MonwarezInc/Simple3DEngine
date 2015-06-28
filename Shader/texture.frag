//GLSL version
#version 330

const int MAX_POINT_LIGHT = 6;

//Input 

in vec2 coordTexture;
in vec3	Normal0;
in vec3	WorldPos0;

#define ONLY_DIRECTIONAL
// Positional Light
struct BaseLight
{
	vec3	Color;
	float	AmbientIntensity;
	float	DiffuseIntensity;
};
#ifdef ONLY_DIRECTIONAL
struct DirectionalLight
{
	vec3 	Color;
	float	AmbientIntensity;
	float	DiffuseIntensity;
	vec3	Direction;
};
#else
struct DirectionalLight
{
	BaseLight	Base;
	vec3		Direction;
};
#endif
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
#ifndef ONLY_DIRECTIONAL
uniform int 				NumPointLights;
uniform PointLight			PointLights[MAX_POINT_LIGHT];			
#endif
uniform DirectionalLight 	directionalLight;
uniform vec3				eyeWorldPos;
uniform float				matSpecularIntensity;
uniform float				matSpecularPower;
// dont forget sampler2D text
uniform sampler2D 			text;
// Output
layout (location = 0) out vec4 out_Color; // for 330 and more


// Fonction main
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
#ifndef ONLY_DIRECTIONAL
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
	float	Attenuation		=	PointLights[Index].Atten.Constant +
								PointLights[Index].Atten.Linear * Distance +
								PointLights[Index].Atten.Exp * Distance * Distance;
	return	Color / Attenuation;
}
#endif
// Fonction main

void main()
{
	#ifdef ONLY_DIRECTIONAL
	// Ambient color
	vec4 	ambientColor	=	vec4(directionalLight.Color,1.0f)*directionalLight.AmbientIntensity;
	vec3	LightDirection	=	-directionalLight.Direction;
	vec3	Normal			=	normalize(Normal0);
	// diffuse factor
	float diffuseFactor		=	dot(Normal, LightDirection);
	vec4 diffuseColor		=	vec4(0,0,0,0);
	vec4 specularColor		=	vec4(0,0,0,0);
	if (diffuseFactor > 0)
	{
		diffuseColor = vec4(directionalLight.Color, 1.0f)* directionalLight.DiffuseIntensity*diffuseFactor;
		vec3	VertexToEye		=	normalize(eyeWorldPos - WorldPos0);
		vec3	LightReflect	=	normalize(reflect(directionalLight.Direction, Normal));
		float	SpecularFactor	=	dot (VertexToEye, LightReflect);
		SpecularFactor			=	pow(SpecularFactor,matSpecularPower);
		if (SpecularFactor > 0)
		{
			specularColor	=	vec4(directionalLight.Color,1.0f)*matSpecularIntensity*SpecularFactor;
		}
	}
   	// Couleur du pixel
   	out_Color	= texture2D(text, coordTexture.xy)* (ambientColor + diffuseColor + specularColor)  ;
	#else
	vec3 	Normal		=	normalize(Normal0);
	vec4	TotalLight	=	CalcDirectionalLight(Normal);
	for	(int i = 0; i < NumPointLights  && i < MAX_NUM_LIGHT; ++i)
	{
		TotalLight +=	CalcPointLight(i, Normal);
	}
	out_Color	=	texture2D(text,coordTexture.xy)*TotalLight;
	#endif	
	
}

