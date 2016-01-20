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
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW for all platform
#include <GL/glew.h>
#include <vector>

namespace S3DE
{
	struct DirectionalLight
	{
		glm::vec3	Color;
		GLfloat		AmbientIntensity;
		glm::vec3	Direction;
		GLfloat		DiffuseIntensity;
	};
	struct BaseLight
	{
		glm::vec3	Color;
		GLfloat		AmbientIntensity;
		GLfloat		DiffuseIntensity;
	};
	struct PointLight	:	public BaseLight
	{
		glm::vec3		Position;
		struct
		{
			GLfloat		Constant;
			GLfloat		Linear;
			GLfloat		Exp;
		} Attenuation;
	};

	class Light : public Shader
	{
		public:
			static 	const	unsigned	int	MAX_POINT_LIGHTS	=	6;
			Light();
			Light(std::string const &vertexSource , std::string const &fragmentSource);
			Light(Light const &);
		
			~Light();
			virtual	void	Show();
			virtual void 	SetDirectionalLight(const DirectionalLight &light);
			virtual	void	SetEyeWorldPos(const glm::vec3 &eyeWorldPos);
			virtual	void	SetMatSpecularIntensity(GLfloat intensity);
			virtual	void	SetMatSpecularPower(GLfloat power);
			virtual	void	SetPointLights(std::vector<PointLight> const &  lights);
			virtual	void	Init();

		protected:
			DirectionalLight	m_dlight;
			glm::vec3			m_eyeWorldPos;
			GLfloat				m_specularIntensity;
			GLfloat				m_specularPower;
			// Location
			GLuint				m_dirLightColorLocation;
			GLuint				m_dirLightAmbientIntensityLocation;
			GLuint				m_dirLightDirectionLocation;
			GLuint				m_dirLightDiffuseIntensityLocation;
			GLuint				m_eyeWorldPosLocation;
			GLuint				m_matSpecularIntensityLocation;
			GLuint				m_matSpecularPowerLocation;
			struct PointLightLocation{
				GLuint			Color;
				GLuint			AmbientIntensity;
				GLuint			DiffuseIntensity;
				GLuint			Position;
				struct
				{
					GLuint		Constant;
					GLuint		Linear;
					GLuint		Exp;
				} Atten;
			}m_pointLightsLocation[MAX_POINT_LIGHTS]; 
			GLuint				m_numPointLightsLocation;
	};
}
