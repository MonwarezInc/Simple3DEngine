#include <iostream>
#include <GraphicEngine/Engine-main.h>
#include <GraphicEngine/Mesh.h>

#include <stdio.h>
#include <cmath>
#include "main.h"
#include <GraphicEngine/Camera.h>
using namespace std;
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;

	cout << "Test engine " << endl;
	
	GraphicEngine::CEngine	engine;

	// Input
	CInput	input;

	// quick loader system
	// not optimal and safe 
	glm::vec3	cPosition;
	glm::vec3	cTarget;
	glm::vec3	cVert(0,0,1);
	int			screenX		=	640;
	int			screenY		=	480;
	bool		fullscreen	=	0;
	try
	{
		FileManager	file("./data/command.dat","r"); // camera settings and time of life
		// typical command file are just
		// camera position(x,y,z) target(x,y,z) up(x,y,z)
		// lifetime t
		// rotate a
		float 	x1,y1,z1,x2,y2,z2,x3,y3,z3;
		int		w,h,f;
		if (fscanf(file.GetFilePtr(),"camera position(%f,%f,%f) target(%f,%f,%f) up(%f,%f,%f)\nresolution %dx%d\nfullscreen %d",
								&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3,&w,&h,&f) != 12)
		{
			x1	=	350;	y1	=	200;	z1	=	300;
			x2	=	2;		y2	=	5;		z2	=	0;
			x3	=	0;		y3	=	0;		z3	=	1;
			w	=	640;	h	=	480;
			f	=	0;
		}
		cPosition	=	glm::vec3(x1,y1,z1);
		cTarget		=	glm::vec3(x2,y2,z2);
		cVert		=	glm::vec3(x3,y3,z3);

		screenX		=	w;
		screenY		=	h;
	
		fullscreen	=	(f != 0)?true:false;
	}
	catch (...)
	{
		std::cerr << "error when loading command.dat" << std::endl;
	}		
	std::vector<unsigned int>			vIDMesh;
	try
	{
		
		engine.CreateWindow(screenX,screenY,fullscreen,"Test Engine", 32, 2, 3,2);
		engine.ClearColor(0.0,0.0,0.0,1.0);
			
		FileManager	file("./data/obj.dat","r");
		BasicVectorManager<GraphicEngine::Mesh>	mesh;
		// first line nb of models
		// then just modelPath position(x,y,z) rotate(a,b,c) scale(f)
		int	nbModel;
		if (fscanf(file.GetFilePtr(),"models %d\n", &nbModel))
		{
			nbModel	=	max(0,nbModel);

			mesh.Allocate(nbModel);
			
			vIDMesh.resize(nbModel);

			engine.SetCameraSettings(70.0, 16/9.0, 0.01, 10000);
			engine.SetCameraLocation(cPosition, cTarget, cVert);
			
			for (size_t i=0; i < nbModel; ++i)
			{
				char	filePath[256];
				float	x,y,z,a,b,c,f;
				if (fscanf(file.GetFilePtr(),"%255s position(%f,%f,%f) rotate(%f,%f,%f) scale(%f)\n", filePath,&x,&y,&z,&a,&b,&c,&f) == 8)
				{
					unsigned int 	id	=	0;
					auto	mesh_ptr	=	mesh.GetVectPtr(i);
					mesh_ptr->LoadFromFile(filePath);
					engine.AddMeshNode(mesh_ptr, id);
					vIDMesh[i]			=	id;
					
					engine.SetNodePosRot(vIDMesh[i], glm::vec3(x,y,z), glm::vec3(a*M_PI/180.0, b*M_PI/180.0, c*M_PI/180.0));
					engine.SetNodeScale(vIDMesh[i], f);
				}
				else
				{	
					file.Release();
					throw std::string ("error data map");	
				}
			}
		}
		Camera	camera(cPosition,cTarget,cVert);
		// we dont need file open anymore
		file.Release();
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
		DirectionalLight	dlight;
		dlight.Color				=	glm::vec3(0.5,0.5,0.8);
		dlight.AmbientIntensity		=	0.4;
		dlight.Direction			=	glm::vec3(1.0,1.0,1.0);
		dlight.DiffuseIntensity		=	0.6;
		light->SetMatSpecularIntensity(1.0);
		light->SetMatSpecularPower(32);
		engine.AttachLight(light);
		// End adding some light		
		while (!input.terminer())
		{
			input.UpdateEvent();
			camera.keyBoardEvent(input);	
			camera.deplacer(input,elapsed);
			unsigned int begin = SDL_GetTicks();
			// do graphical stuff
			// do animation 
			// light animation
			dlight.Direction	=	glm::vec3(std::cos(totalTime),std::sin(totalTime),-1.0);
			light->SetDirectionalLight(dlight);
			//
			engine.SetCameraLocation(camera.GetPosition(),camera.GetPointCible(),cVert);
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

