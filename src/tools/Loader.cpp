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
#include <S3DE_Loader.h>
#include <fstream>
#include <iostream>
#include <S3DE_Parser.h>
using std::string;
using std::vector;
using namespace S3DE;
// Some Bits operation define
#define CONFIG_MASK	1		//	00000001
#define MESH_MASK 	2		//	00000010
#define LIGHT_MASK 	4		//	00000100
#define DYN_MASK	8		//	00001000
Loader::Loader()
{
	m_state	=	0;
}
void Loader::Load(string const &filename, LoaderType type)
{
	m_lastfilename =	filename;
	switch (type)
	{
		case LoaderType::CONFIG:
			this->LoadConfig();
			break;
		case LoaderType::MESH:
			this->LoadMesh();
			break;
		case LoaderType::LIGHT:
			this->LoadLight();
			break;
		case LoaderType::DYNAMICS:
			this->LoadDynamics();
			break;
	}
}
void Loader::LoadConfig()
{
	std::ifstream	input(m_lastfilename.c_str());
	this->ClearState(CONFIG_MASK);
	if (input.is_open())
	{
		float	x,y,z,u,v,w,a,b,c;
		std::string	buf;
		std::getline(input, buf);
		auto	i1	=	buf.find("camera position");
		if (i1	==	std::string::npos)
			throw string("Error configuration file at first line");

		std::string	substr1;
		auto	i2	=	Parser::ExtractMatch(buf,substr1);
		Parser::Find3uple(substr1,x,y,z);
		buf	=	buf.substr(i2+1);	
		
		i1	=	buf.find("target");
		if (i1	==	std::string::npos)
			throw string("Error configuration file at first line");
		buf	=	buf.substr(i1);
		i2		=	Parser::ExtractMatch(buf,substr1);
		Parser::Find3uple(substr1,u,v,w);
		
		buf	=	buf.substr(i2+1);
		i1	=	buf.find("up");
		if (i1	==	std::string::npos)
			throw string("Error configuration file at first line");
		Parser::ExtractMatch(buf,substr1);
		Parser::Find3uple(substr1,a,b,c);

		std::getline(input,buf);
		i1	=	buf.find("resolution");
		if (i1	==	std::string::npos)
			throw string("Error configuration file at first line");
		Parser::ExtractMatch(buf,substr1);
		unsigned long	width,height;
		Parser::FindCouple(substr1,width,height);

		std::getline(input,buf);
		i1	=	buf.find("fullscreen");
		if (i1	==	std::string::npos)
			throw string("Error configuration file at first line");
		Parser::ExtractMatch(buf,substr1);
		auto	fullscreen	=	std::stoul(substr1);

		// Now we can store the config
		m_config.position	=	glm::vec3(x,y,z);
		m_config.target		=	glm::vec3(u,v,w);
		m_config.up			=	glm::vec3(a,b,c);
		m_config.width		=	width;
		m_config.height		=	height;
		m_config.fullscreen	=	(1==fullscreen)?true:false;	
		// Update state
		m_state				|=	CONFIG_MASK;									
	}
}
void Loader::LoadMesh()
{
	std::ifstream	input(m_lastfilename.c_str());
	this->ClearState(MESH_MASK);
	m_pMesh.clear();

	string error	=	string("error during loading meshfile: ") + m_lastfilename;
	if (input.is_open())
	{
		std::string buf;
		do
		{
			std::getline(input,buf);
			buf	=	buf.substr(0,buf.find("#"));
			if (buf.length() != 0)
			{
				auto	i1		=	buf.find(" ");
				if ((i1 == std::string::npos) || (i1 == 0))
					throw error;
				auto	substr	=	buf.substr(0,i1);
				auto	name	=	substr;

				buf	=	buf.substr(i1+1);
				i1	=	buf.find(" ");
				if ((i1 == std::string::npos) || (i1 == 0))
					throw error;
				auto	entity	=	buf.substr(0,i1);
				buf	=	buf.substr(i1);

				i1	=	buf.find("position");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				auto i2	=	Parser::ExtractMatch(buf,substr);
				float x,y,z,u,v,w,f;
				Parser::Find3uple(substr,x,y,z);
				buf	=	buf.substr(i2+1);
				
				i1	=	buf.find("rotate");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				i2	=	Parser::ExtractMatch(buf,substr);
				Parser::Find3uple(substr,u,v,w);
				buf	=	buf.substr(i2+1);
			
				i1	=	buf.find("scale");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				Parser::ExtractMatch(buf,substr);
				f	=	std::stof(substr);
				
				m_pMesh.push_back(MeshData());
				m_pMesh.back().filename	=	name;
				m_pMesh.back().position	=	glm::vec3(x,y,z);
				m_pMesh.back().pitch	=	glm::vec3(glm::radians(u),glm::radians(v),glm::radians(w));
				m_pMesh.back().scale	=	f;
				
				// Everything goes well
				m_state	|= MESH_MASK;
			}
		}while (!input.eof());
	}
}
void Loader::LoadLight()
{
	std::ifstream	input(m_lastfilename.c_str());
	this->ClearState(LIGHT_MASK);
	m_vLight.clear();

	string	error	=	string("error during loading lights file: ") + m_lastfilename;
	if (input.is_open())
	{
		std::string buf;
		std::string	controltype	=	"unknow";
		do
		{
			std::getline(input,buf);
			buf	=	buf.substr(0,buf.find("#"));
			auto i1 = buf.find("color");
			auto j1	= buf.find("controlpoint");
			auto k1	= buf.find("position");
			if ((buf.length() != 0) && (i1 != std::string::npos))
			{
				controltype	=	"unknow"; // Reset controltype
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				std::string	substr;
				auto	i2	=	Parser::ExtractMatch(buf,substr);
				float r,g,b,a,d,c,l,e;
				Parser::Find3uple(substr,r,g,b);
				buf	=	buf.substr(i2+1);

				i1 = buf.find("ambiant");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				i2	=	Parser::ExtractMatch(buf,substr);
				a	=	std::stof(substr);
				buf	=	buf.substr(i2+1);
				
				i1 = buf.find("diffuse");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				i2	=	Parser::ExtractMatch(buf,substr);
				d	=	std::stof(substr);
				buf	=	buf.substr(i2+1);

				i1 = buf.find("linear");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				i2	=	Parser::ExtractMatch(buf,substr);
				l	=	std::stof(substr);
				buf	=	buf.substr(i2+1);

				i1 = buf.find("constant");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				i2	=	Parser::ExtractMatch(buf,substr);
				c	=	std::stof(substr);
				buf	=	buf.substr(i2+1);

				i1 = buf.find("exp");
				if (i1 == std::string::npos)
					throw error;
				buf	=	buf.substr(i1);
				i2	=	Parser::ExtractMatch(buf,substr);
				e	=	std::stof(substr);
				buf	=	buf.substr(i2+1);
			
				// Set the data in m_vLight
				m_vLight.push_back(LightData());
				m_vLight.back().color	=	glm::vec3(r,g,b);
				m_vLight.back().ambient	=	a;
				m_vLight.back().diffuse	=	d;
				m_vLight.back().linear	=	l;
				m_vLight.back().constant=	c;
				m_vLight.back().exp		=	e;
			}
			else if ((buf.length() != 0) && (j1 != std::string::npos))
			{
				if (buf.find("linear") != std::string::npos)
					controltype	=	"linear";
				else
					controltype	=	"unknow";
				m_vLight.back().controltype	=	controltype;	
			}
			else if ((buf.length() != 0) && (k1 != std::string::npos))
			{
				if (controltype	==	"linear")
				{
					float time,x,y,z;
					buf	=	buf.substr(k1);
					std::string	substr;
					auto k2	=	Parser::ExtractMatch(buf,substr);
					Parser::Find3uple(substr,x,y,z);
					buf	=	buf.substr(k2+1);
					
					k1	=	buf.find("timemill");
					if (k1 == std::string::npos)
						throw error;
					buf	=	buf.substr(k1);
					Parser::ExtractMatch(buf,substr);
					time	=	stof(substr);
					m_vLight.back().vControlPoint.push_back(ControlPoint());
					m_vLight.back().vControlPoint.back().position	=	glm::vec3(x,y,z);
					m_vLight.back().vControlPoint.back().time		=	time;
					
				}
				else
				{
					// For now only linear is supported
				}
			}
		}
		while(!input.eof());

		// Everything goes well
		m_state |= LIGHT_MASK;
	}
}
void Loader::LoadDynamics()
{
	std::ifstream	input(m_lastfilename.c_str());
}
ConfigData	Loader::GetConfigData()
{
	if (0 == (m_state & CONFIG_MASK))
		throw string ("error can't get data config before loading it");
	return	m_config;
}
vector<MeshData> Loader::GetMeshData()
{
	if (0 == (m_state & MESH_MASK))
		throw string ("error can't get mesh config before loading it");
	return m_pMesh;
}
vector<LightData>	Loader::GetLightData()
{
	if (0 == (m_state & LIGHT_MASK))
		throw string ("error can't get light config before loading it");
	return	m_vLight;
}
void	Loader::ClearState(unsigned char mask)
{
	if (0 != (m_state & mask))
		m_state -= mask;
}
