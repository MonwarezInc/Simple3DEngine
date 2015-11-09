/*
Copyright (C) 2014-2015 Payet thibault

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
#ifndef LOADER_HEADER_INCLUED_H
#define LOADER_HEADER_INCLUED_H
#include "FileManager.h"
#include <glm/glm.hpp>
#include <cstdio>
#include <string>
enum Loader_Type
{ LOADER_CONFIG, LOADER_MESH, LOADER_LIGHT, LOADER_DYNAMICS };
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
		void					Load(std::string const &filename, Loader_Type type);
		ConfigData				GetConfigData();
		std::vector<MeshData>	GetMeshData();
		std::vector<LightData> 	GetLightData();
	protected:
		void	LoadConfig(FileManager &file);
		void	LoadMesh(FileManager &file);
		void 	LoadLight(FileManager &file);
		void	LoadDynamics(FileManager &file);
		void	ClearState(unsigned char mask);

		std::string				m_lastfilename;
		unsigned char			m_state;
		ConfigData				m_config;
		std::vector<MeshData>	m_pMesh;
		std::vector<LightData>	m_vLight;	
};
#endif
