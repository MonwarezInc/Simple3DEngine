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
#include <iostream>
#include <S3DE/Engine-main.h>
#include <S3DE/Mesh.h>

#include <cstdio>
#include <cmath>
#include <sstream>
#include "utils/MemoryManager.hpp"
#include "utils/Interpolate.hpp"
#include "utils/Loader.h"
#include <S3DE/Camera.h>

#define MAX_LIGHT 6 	// define this for now
struct IdMesh
{
	IdMesh()
	{
		id		=	0;
		isGood	=	false;
	}
	unsigned	int	id;
	bool			isGood;
};
using namespace std;
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;

	cout << "Test engine " << endl;
	
	S3DE::CEngine	engine;

	// Input
	CInput	input;

	// loader system
	Loader	loader;
	// Some struct for loader system
 	ConfigData			config;
	vector<MeshData>	pmeshdata;
	// Set some default
	config.position		=	glm::vec3(350,200,300);
	config.target		=	glm::vec3(2,5,0);
	config.up			=	glm::vec3(0,0,1);
	config.width		=	640;
	config.height		=	480;
	config.fullscreen	=	false;

	try
	{
		loader.Load("./data/config",LOADER_CONFIG);
	}
	catch (string a)
	{
		std::cerr << a << std::endl;
	}
	std::vector<IdMesh>			vIDMesh;
	try
	{
		config	=	loader.GetConfigData();
		engine.CreateWindow(config.width,config.height,config.fullscreen,"Test Engine", 32, 2, 3,2);
		engine.ClearColor(0.0,0.0,0.0,1.0);
		engine.SetCameraSettings(70.0, (float)config.width/(float)config.height, 0.01, 10000);
		engine.SetCameraLocation(config.position, config.target, config.up);

		loader.Load("./data/obj.dat",LOADER_MESH);
		pmeshdata	=	loader.GetMeshData();	
		BasicVectorManager<S3DE::Mesh>	mesh;
		auto	nbModel	=	pmeshdata.size();
		mesh.Allocate(nbModel);
		vIDMesh.resize(nbModel);
		for (size_t	i = 0; i < nbModel ; ++i)
		{
			unsigned int	id	=	0;
			auto mesh_ptr	=	mesh.GetVectPtr(i);
			try
			{
				mesh_ptr->LoadFromFile(pmeshdata[i].filename);
				engine.AddMeshNode(mesh_ptr, id);
				vIDMesh[i].id		=	id;
				vIDMesh[i].isGood	=	true;

				engine.SetNodePosRot(vIDMesh[i].id, pmeshdata[i].position, pmeshdata[i].pitch);
				engine.SetNodeScale(vIDMesh[i].id, pmeshdata[i].scale);
			}
			catch(std::string const & a)
			{
				std::stringstream out;
				out << "Exception caught when loading: " << pmeshdata[i].filename ;
				std::cerr << out.str() << std::endl << a << std::endl;
			}
			catch (...)
			{
				throw;
			}
		}
		Camera	camera(config.position,config.target,config.up);
		// maybe we should encapsulate timer 
		unsigned int 	start		=	SDL_GetTicks();
		unsigned int 	frametime	=	16;
		unsigned int	elapsed		=	0;
		unsigned int	totalTime	=	SDL_GetTicks() - start;
		input.GrabCursor(true);
		input.ShowCursor(false);	
		
		camera.setSpeed(0.1);
		// Adding some light
		std::vector<LightData>					lightdata;
		std::vector<S3DE::PointLight>			pointlight;
		std::vector<LinearInterpolate<float>>	posintlight;
		try
		{
			loader.Load("./data/light.dat", LOADER_LIGHT);
			lightdata	=	loader.GetLightData();
			size_t	nlights	=	lightdata.size();
			for (size_t i = 0; i < nlights && i < MAX_LIGHT; ++i)
			{
				S3DE::PointLight	pl;
				pl.Color				=	lightdata[i].color;
				pl.AmbientIntensity		=	lightdata[i].ambient;
				pl.DiffuseIntensity		=	lightdata[i].diffuse;
				pl.Attenuation.Constant	=	lightdata[i].constant;
				pl.Attenuation.Linear	=	lightdata[i].linear;
				pl.Attenuation.Exp		=	lightdata[i].exp;
				if (lightdata[i].controltype	==	"linear")
				{
					size_t controlpoint	=	lightdata[i].vControlPoint.size();
					posintlight.push_back(LinearInterpolate<float>());
					for (size_t j = 0; j < controlpoint; ++j)
					{
						auto vec	=	lightdata[i].vControlPoint[j].position;
						Position3D<float>	position(vec.x,vec.y,vec.z);
						posintlight.back().AddPoint(position, lightdata[i].vControlPoint[j].time);
					}
				}
				else	// throw except
				{
					throw string ("Error: ") + lightdata[i].controltype + string(" curve not implemented");
				}
				// Set looped for beginning
				posintlight.back().SetLooped(true);
				pointlight.push_back(pl);
			}
		}
		catch(string const &err)
		{
			std::cerr << "Uncategorized error" << std::endl;
			std::cerr << err << std::endl;
		}
		engine.AttachLight(pointlight);
		// End adding some light
		float	t	=	0;
		while (!input.terminer())
		{
			auto numLight	=	pointlight.size();
			for (size_t i = 0; i < numLight; ++i)
			{
				// Apply Interpolated curve position
				Position3D<float>	lightpos	=	posintlight[i].GetInterpolated(totalTime);
				pointlight[i].Position	=	glm::vec3(lightpos.x,lightpos.y,lightpos.z);	
			}
			engine.AttachLight(pointlight);

			input.UpdateEvent();
			camera.keyBoardEvent(input);	
			camera.deplacer(input,elapsed);
			unsigned int begin = SDL_GetTicks();
			// do graphical stuff
			// do animation 
			engine.SetCameraLocation(camera.GetPosition(),camera.GetPointCible(),config.up);
			engine.Init();
			engine.Draw(totalTime);
			elapsed = SDL_GetTicks() - begin;
			if (elapsed < frametime)
				{
					SDL_Delay(frametime - elapsed);
					elapsed	=	SDL_GetTicks() - begin;
				}
			if (input.GetTouche(SDL_SCANCODE_ESCAPE))
				break;
		totalTime	=	SDL_GetTicks() - start;
		t	+=	elapsed/1800.0;
		}
	}
	catch(string const &a)
	{
		std::cerr << "Error"<<std::endl << a << std::endl;
	}
	catch(...)
	{
		std::cerr << "unexpected error " << std::endl;
	}
	return EXIT_SUCCESS;
}

