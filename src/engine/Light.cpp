/*
Copyright (c) 2016, Payet Thibault
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Monwarez Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <S3DE/Light.h>
#include <cstdio>
#include <sstream>
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
		std::stringstream	out;

		out <<"PointLights["<< i <<"].Base.Color";
		m_pointLightsLocation[i].Color				=	GetUniformLocation(out.str());
		out.str("");

		out <<"PointLights["<< i <<"].Base.AmbientIntensity";
		m_pointLightsLocation[i].AmbientIntensity	=	GetUniformLocation(out.str());
		out.str("");
		
		out <<"PointLights["<< i <<"].Position";
		m_pointLightsLocation[i].Position			=	GetUniformLocation(out.str());
		out.str("");
		
		out <<"PointLights["<< i <<"].Base.DiffuseIntensity";
		m_pointLightsLocation[i].DiffuseIntensity	=	GetUniformLocation(out.str());
		out.str("");
		
		out <<"PointLights["<< i <<"].Atten.Constant";
		m_pointLightsLocation[i].Atten.Constant		=	GetUniformLocation(out.str());
		out.str("");
		
		out <<"PointLights["<< i <<"].Atten.Linear";
		m_pointLightsLocation[i].Atten.Linear		=	GetUniformLocation(out.str());
		out.str("");
		
		out <<"PointLights["<< i <<"].Atten.Exp";
		m_pointLightsLocation[i].Atten.Exp			=	GetUniformLocation(out.str());
	}

}
