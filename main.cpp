#include <iostream>
#include "engine/Engine-main.h"
#include "engine/Mesh.h"
#include "engine/MiscObject.h"

#include <stdio.h>

using namespace std;
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;

	cout << "Test engine " << endl;
	
	GraphicEngine::CEngine	engine;
	engine.CreateWindow(800,600,false,"Test Engine", 32, 2, 2,1);
	engine.ClearColor(0.0,0.0,0.0,1.0);

	// quick loader system
	// not optimal and safe 
	FILE*	file= nullptr;
	file	=	fopen("./data/obj.dat","r");
	std::vector<GraphicEngine::Mesh*>	vMesh;
	std::vector<unsigned int>		vIDMesh;
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
	
				engine.SetCameraSettings(70.0, 800/600.0, 0.01, 10000);
				engine.SetCameraLocation(glm::vec3(250,0,20), glm::vec3(0,0,0), glm::vec3(0,0,1));
				
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
						throw std::string ("error data map");	
				}
			}
			fclose(file);
		}

		// maybe we should encapsulate timer 
		//unsigned int start	=	SDL_GetTicks();
		unsigned int frametime	=	16;
		unsigned int elapsed	=	0;
		while (SDL_GetTicks() < 5000)
		{
			unsigned int begin = SDL_GetTicks();
			// do graphical stuff
			//engine.Clear();
			engine.Init();
			engine.Draw(elapsed);
			elapsed = SDL_GetTicks() - begin;
			if (elapsed < frametime)
				{
					SDL_Delay(frametime - elapsed);
					elapsed	=	SDL_GetTicks() - begin;
				}
			
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

