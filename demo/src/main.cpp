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
#include "utils/MemoryManager.hpp"
#include "utils/Interpolate.hpp"
#include "utils/Loader.h"
#include <S3DE/Camera.h>

#define MAX_LIGHT 6 	// define this for now
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
	std::vector<unsigned int>			vIDMesh;
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
			mesh_ptr->LoadFromFile(pmeshdata[i].filename);
			engine.AddMeshNode(mesh_ptr, id);
			vIDMesh[i]		=	id;

			engine.SetNodePosRot(vIDMesh[i], pmeshdata[i].position, pmeshdata[i].pitch);
			engine.SetNodeScale(vIDMesh[i], pmeshdata[i].scale);
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
		auto 				light	=	std::make_shared<Light>();
		std::vector<PointLight>			pointlight;
		std::vector<LinearInterpolate<float>>	posintlight;
		FileManager	file;
		try
		{
			file.LoadFile("./data/light.dat","r");
			unsigned int nblights	=	0;
			if (fscanf(file.GetFilePtr(),"lights %u\n",&nblights) == 1)
			{
				for (auto i = 0; i < nblights && i < MAX_LIGHT; ++i)
				{
					PointLight	pl;
					float x,y,z,r,g,b,a,d,l,c,e;
					int	controlpoint(0);
					char	curvetype[256];
					if (fscanf(file.GetFilePtr(),
						"color(%f,%f,%f) ambiant(%f) diffuse(%f) linear(%f) constant(%f) exp(%f)\n",
						&r,&g,&b,&a,&d,&l,&c,&e) != 8)
						throw std::string("error in the first line of light");
					if (fscanf(file.GetFilePtr(),"controlpoint(%d) %255s\n", &controlpoint,curvetype) != 2)
						throw std::string("error in the controlpoint line");
					// set static settings first
					pl.Color				=	glm::vec3(r,g,b);
					pl.AmbientIntensity		=	a;
					pl.DiffuseIntensity		=	d;
					pl.Attenuation.Constant	=	c;
					pl.Attenuation.Linear	=	l;
					pl.Attenuation.Exp		=	e;
					std::string scurvetype(curvetype);
					if (scurvetype == "linear") // We only manage linear curve for now
					{
						posintlight.push_back(LinearInterpolate<float>());
						for (auto j = 0; j < controlpoint; ++j)
						{
							float time;
							if(fscanf(file.GetFilePtr(),"position(%f,%f,%f) timemill(%f)\n",&x,&y,&z,&time) != 4)
								throw std::string("error in controlpoint line position");
							posintlight.back().AddPoint(Position3D<float>(x,y,z), time);	
						}
					}
					else
					{
						// We consider it's linear for now
						posintlight.push_back(LinearInterpolate<float>());
						float time;
						if(fscanf(file.GetFilePtr(),"position(%f,%f,%f) timemill(%f)\n",&x,&y,&z,&time) != 4)
							throw std::string("error in controlpoint line position");
						posintlight.back().AddPoint(Position3D<float>(x,y,z), time);	
					}
					// Set looped for posintlight
					posintlight.back().SetLooped(true);
					pointlight.push_back(pl);
				}
			}
			else
			{
				throw std::string("error lights number of lights");
			}
			file.Release();
		}
		catch(std::string err)
		{
			std::cerr << "Uncategorized error" << std::endl;
			std::cerr << err << std::endl;
			file.Release();
		}
		catch(...)
		{
			std::cerr << "Error during loading ./data/lights.dat" << std::endl;
			file.Release();
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
		std::cerr << "erreur " << a << std::endl;
	}
	catch(...)
	{
		std::cerr << "unexpected error " << std::endl;
	}
	return EXIT_SUCCESS;
}

