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
#include "Loader.h"
using std::string;
// Some Bits operation define
#define CONFIG_MASK	1
#define MESH_MASK 	2
#define LIGHT_MASK 	4
#define DYN_MASK	8
Loader::Loader()
{
	m_state	=	0;
}
void Loader::Load(string const &filename, Loader_Type type)
{
	FileManager	file (filename, "r");
	m_lastfilename =	filename;
	switch (type)
	{
		case LOADER_CONFIG:
			this->LoadConfig(file);
			break;
		case LOADER_MESH:
			this->LoadMesh(file);
			break;
		case LOADER_LIGHT:
			this->LoadLight(file);
			break;
		case LOADER_DYNAMICS:
			this->LoadDynamics(file);
			break;
	}
}
void Loader::LoadConfig(FileManager & file)
{
	float	x,y,z,u,v,w,a,b,c;
	auto ret	=	fscanf(file.GetFilePtr(), "camera position(%f,%f,%f) target(%f,%f,%f) up(%f,%f,%f)\n",
												&x,&y,&z,&u,&v,&w,&a,&b,&c);
	if (ret != 9)
		throw string ("error during loading config file at: ") + m_lastfilename;
	unsigned int	width,height,fullscreen;
	ret			=	fscanf(file.GetFilePtr(),"resolution %ux%u\nfullscreen %u",&width,&height,&fullscreen);
	if	(ret != 3)
		throw string ("error during loading config file at: ") + m_lastfilename;
	// Now we can store the config
	m_config.position	=	glm::vec3(x,y,z);
	m_config.target		=	glm::vec3(u,v,w);
	m_config.up			=	glm::vec3(a,b,c);
	m_config.width		=	width;
	m_config.height		=	height;
	m_config.fullscreen	=	(1==fullscreen)?true:false;	
	// Update state
	m_state	=	m_state | CONFIG_MASK;									
}
void Loader::LoadMesh(FileManager & file)
{
}
void Loader::LoadLight(FileManager & file)
{
}
void Loader::LoadDynamics(FileManager & file)
{
}
ConfigData	Loader::GetConfigData()
{
	if (1 != (m_state & CONFIG_MASK))
		throw string ("error can't get config data before loading it");
	return	m_config;
}
