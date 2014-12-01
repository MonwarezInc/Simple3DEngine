#include <iostream>
#include "engine/Engine-main.h"
#include "engine/Mesh.h"
#include "engine/MiscObject.h"

#include <stdio.h>
#include <cmath>

using namespace std;
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;

	cout << "Test engine " << endl;
	
	GraphicEngine::CEngine	engine;
	engine.CreateWindow(1600,900,true,"Test Engine", 32, 2, 2,1);
	engine.ClearColor(0.0,0.0,0.0,1.0);

	// quick loader system
	// not optimal and safe 
	FILE*	file= nullptr;
	file	=	fopen("./data/command.dat","r"); // camera settings and time of life
	glm::vec3	cPosition;
	glm::vec3	cTarget;
	glm::vec3	cVert;
	int			lifetime	=	2000;
	float		dangle		=	0;

	if (nullptr != file)
	{
		// typical command file are just
		// camera position(x,y,z) target(x,y,z) up(x,y,z)
		// lifetime t
		// rotate a
		float 	x1,y1,z1,x2,y2,z2,x3,y3,z3,a;
		int		t;
		if (fscanf(file,"camera position(%f,%f,%f) target(%f,%f,%f) up(%f,%f,%f)\nlifetime %d\nrotate %f",
								&x1,&y1,&z1,&x2,&y2,&z2,&x3,&y3,&z3,&t,&a) != 11)
		{
			x1	=	350;	y1	=	200;	z1	=	300;
			x2	=	2;		y2	=	5;		z2	=	0;
			x3	=	0;		y3	=	0;		z3	=	1;
			t	=	5000;
			a	=	45.0;
		}
		fclose(file);
		file	=	nullptr;

		cPosition	=	glm::vec3(x1,y1,z1);
		cTarget		=	glm::vec3(x2,y2,z2);
		cVert		=	glm::vec3(x3,y3,z3);

		lifetime	=	t;
		dangle		=	a;

	}
	else
	{
		std::cerr << "error when loading command.dat" << std::endl;
	}		
	file	=	fopen("./data/obj.dat","r");
	std::vector<GraphicEngine::Mesh*>	vMesh;
	std::vector<unsigned int>			vIDMesh;
	try
	{
		if (nullptr	!= file)
		{
			
			// first line nb of models
			// then just modelPath position(x,y,z) rotate(a,b,c) scale(f)
			int	nbModel;
			if (fscanf(file,"models %d\n", &nbModel))
			{
				nbModel	=	max(0,nbModel);

				vMesh.resize(nbModel);
				for (unsigned int i =0; i < (unsigned int)nbModel; ++i)
					vMesh[i]	=	new	GraphicEngine::Mesh();
				
				vIDMesh.resize(nbModel);
	
				engine.SetCameraSettings(70.0, 16/9.0, 0.01, 10000);
				engine.SetCameraLocation(cPosition, cTarget, cVert);
				
				for (unsigned int i=0; i < nbModel; ++i)
				{
					char	filePath[256];
					float	x,y,z,a,b,c,f;
					if (fscanf(file,"%255s position(%f,%f,%f) rotate(%f,%f,%f) scale(%f)\n", filePath,&x,&y,&z,&a,&b,&c,&f) == 8)
					{
						unsigned int 	id	=	0;
						vMesh[i]->LoadFromFile(filePath);
						engine.AddObject(vMesh[i], id);
						vIDMesh[i]			=	id;
						
						engine.SetObjectPosRot(vIDMesh[i], glm::vec3(x,y,z), glm::vec3(a*M_PI/180.0, b*M_PI/180.0, c*M_PI/180.0));
						engine.SetObjectScale(vIDMesh[i], f);
					}
					else
					{	
						fclose(file);
						file	=	nullptr;	
						throw std::string ("error data map");	
					}
				}
			}
			fclose(file);
		}

		// maybe we should encapsulate timer 
		unsigned int 	start		=	SDL_GetTicks();
		unsigned int 	frametime	=	16;
		unsigned int	elapsed		=	0;
		float			angle		=	0;			
		while (start < lifetime)
		{
			unsigned int begin = SDL_GetTicks();
			// do graphical stuff
			// do animation like moving camera
			float	cosAngle	=	cos(angle);
			float	sinAngle	=	sin(angle);
			glm::vec3	position= 	glm::vec3(cPosition[0] * cosAngle + cPosition[1] * sinAngle, 
												cPosition[1] * cosAngle - cPosition[0] * sinAngle,
												cPosition[2]);
			engine.SetCameraLocation(position,cTarget,cVert);
			engine.Init();
			engine.Draw(elapsed);
			elapsed = SDL_GetTicks() - begin;
			if (elapsed < frametime)
				{
					SDL_Delay(frametime - elapsed);
					elapsed	=	SDL_GetTicks() - begin;
				}
			angle	+= (elapsed*0.001)*(dangle)*(M_PI/180);
			start	=	SDL_GetTicks();
		}
	}
	catch(string a)
	{
		std::cerr << "erreur " << a << std::endl;
	}
	catch(...)
	{
		std::cerr << "unexpected error " << std::endl;
	}
	// free stage
	for (unsigned int i=0; i < vMesh.size(); ++i)
	{
		delete	vMesh[i];
	}
	return EXIT_SUCCESS;
}

