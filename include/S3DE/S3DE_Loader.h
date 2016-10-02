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
#pragma once
#include <glm/glm.hpp>
#include <string>
#include <fstream>
#include <vector>
namespace S3DE
{
enum class LoaderType: char
{ CONFIG, MESH, LIGHT, DYNAMICS };
struct 	ConfigData
{
	glm::vec3		position;
	glm::vec3		target;
	glm::vec3		up;

	unsigned	int	width;
	unsigned	int	height;
	bool			fullscreen;
};
// MeshData is for one mesh
struct 	MeshData
{
	std::string 	filename;
	std::string		entityName;
	glm::vec3		position;
	glm::vec3		pitch;
	float			scale;
};
struct	ControlPoint
{
	glm::vec3					position;
	float						time;	
};
struct 	LightData
{
	glm::vec3					color;
	float						ambient;
	float						diffuse;
	float						linear;
	float						constant;
	float						exp;
	std::string					controltype;
	std::vector<ControlPoint>	vControlPoint;
};
class	Loader
{
	public:
		Loader ();
		void					Load(std::string const &filename, LoaderType type);
		ConfigData				GetConfigData();
		std::vector<MeshData>	GetMeshData();
		std::vector<LightData> 	GetLightData();
	protected:
		void	LoadConfig();
		void	LoadMesh();
		void 	LoadLight();
		void	LoadDynamics();
		void	ClearState(unsigned char mask);

		std::string				m_lastfilename;
		unsigned char			m_state;
		ConfigData				m_config;
		std::vector<MeshData>	m_pMesh;
		std::vector<LightData>	m_vLight;	
};
}  // end of S3DE namespace
