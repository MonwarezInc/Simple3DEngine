/*
Copyright (C) 2014 Payet thibault

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>
*/
#include <S3DE/Light.h>
#include <cstdio>
using namespace S3DE;
Light::Light():Shader()
{
	
}
Light::Light(Light const &shaderToCopy):Shader(shaderToCopy)
{
}
Light::Light(std::string const &vertexSource, std::string const &fragmentSource):Shader(vertexSource,fragmentSource)
{
}
Light::~Light()
{
}
void	Light::SetEyeWorldPos(const glm::vec3 &eye)
{
	m_eyeWorldPos	=	eye;
}
void	Light::SetMatSpecularIntensity(GLfloat intensity)
{
	m_specularIntensity	=	intensity;
}
void	Light::SetMatSpecularPower(GLfloat	power)
{
	m_specularPower	=	power;
}
void	Light::SetDirectionalLight(const DirectionalLight &light)
{
	m_dlight	=	light;
}
void	Light::SetPointLights(std::vector<PointLight> const & lights)
{
	// truncate with MAX_POINT_LIGHTS
	int	numPointLight	=	lights.size();
	numPointLight	=	(numPointLight > MAX_POINT_LIGHTS)? MAX_POINT_LIGHTS:numPointLight;
	glUniform1i(m_numPointLightsLocation,numPointLight);
	for (int i= 0; i < numPointLight; ++i)
	{
		glUniform3f(m_pointLightsLocation[i].Color, lights[i].Color.x,lights[i].Color.y,lights[i].Color.z);
		glUniform1f(m_pointLightsLocation[i].AmbientIntensity,lights[i].AmbientIntensity);
		glUniform1f(m_pointLightsLocation[i].DiffuseIntensity,lights[i].DiffuseIntensity);
		glUniform3f(m_pointLightsLocation[i].Position, lights[i].Position.x,lights[i].Position.y,lights[i].Position.z);
		glUniform1f(m_pointLightsLocation[i].Atten.Constant,lights[i].Attenuation.Constant);
		glUniform1f(m_pointLightsLocation[i].Atten.Linear,lights[i].Attenuation.Linear);
		glUniform1f(m_pointLightsLocation[i].Atten.Exp,lights[i].Attenuation.Exp);
	}
}
void	Light::Show()
{
	// sending light information
	glUniform3f(m_dirLightColorLocation, m_dlight.Color.x, m_dlight.Color.y, m_dlight.Color.z);
	glUniform1f(m_dirLightAmbientIntensityLocation, m_dlight.AmbientIntensity);
	glm::vec3 	direction	=	m_dlight.Direction;
	direction				=	glm::normalize(direction);
	glUniform3f(m_dirLightDirectionLocation,direction.x,direction.y,direction.z);
	glUniform1f(m_dirLightDiffuseIntensityLocation, m_dlight.DiffuseIntensity);
	glUniform3f(m_eyeWorldPosLocation,m_eyeWorldPos.x,m_eyeWorldPos.y,m_eyeWorldPos.z);
	glUniform1f(m_matSpecularIntensityLocation, m_specularIntensity);
	glUniform1f(m_matSpecularPowerLocation, m_specularPower);
}
void	Light::Init()
{
	// Get the location of the light uniform in the shader
	m_dirLightColorLocation				=	GetUniformLocation("directionalLight.Base.Color");
	m_dirLightAmbientIntensityLocation	=	GetUniformLocation("directionalLight.Base.AmbientIntensity");
	m_dirLightDirectionLocation			=	GetUniformLocation("directionalLight.Direction");
	m_dirLightDiffuseIntensityLocation	=	GetUniformLocation("directionalLight.Base.DiffuseIntensity");
	m_eyeWorldPosLocation				=	GetUniformLocation("eyeWorldLocation");
	m_matSpecularIntensityLocation		=	GetUniformLocation("matSpecularIntensity");
	m_matSpecularPowerLocation			=	GetUniformLocation("matSpecularPower");
	
	m_numPointLightsLocation			=	GetUniformLocation("NumPointLights");
	for (unsigned int i=0; i < MAX_POINT_LIGHTS;++i)
	{
		char Name[128];
		memset(Name,0,sizeof(Name));

		snprintf(Name,sizeof(Name),"PointLights[%d].Base.Color",i);
		m_pointLightsLocation[i].Color				=	GetUniformLocation(Name);
		
		snprintf(Name,sizeof(Name),"PointLights[%d].Base.AmbientIntensity",i);
		m_pointLightsLocation[i].AmbientIntensity	=	GetUniformLocation(Name);
		
		snprintf(Name,sizeof(Name),"PointLights[%d].Position",i);
		m_pointLightsLocation[i].Position			=	GetUniformLocation(Name);
		
		snprintf(Name,sizeof(Name),"PointLights[%d].Base.DiffuseIntensity",i);
		m_pointLightsLocation[i].DiffuseIntensity	=	GetUniformLocation(Name);
		
		snprintf(Name,sizeof(Name),"PointLights[%d].Atten.Constant",i);
		m_pointLightsLocation[i].Atten.Constant		=	GetUniformLocation(Name);
		
		snprintf(Name,sizeof(Name),"PointLights[%d].Atten.Linear",i);
		m_pointLightsLocation[i].Atten.Linear		=	GetUniformLocation(Name);
		
		snprintf(Name,sizeof(Name),"PointLights[%d].Atten.Exp",i);
		m_pointLightsLocation[i].Atten.Exp			=	GetUniformLocation(Name);
	}

}
