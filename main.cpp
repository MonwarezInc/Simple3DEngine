#include <iostream>
#include "engine/Engine-main.h"
#include "engine/Mesh.h"
#include "engine/MiscObject.h"

using namespace std;
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;

	cout << "Test engine " << endl;
	
	GraphicEngine::CEngine	engine;
	engine.CreateWindow(800,600,false,"Test Engine", 32, 2, 2,1);
	engine.ClearColor(0.0,0.0,0.0,1.0);
	GraphicEngine::Mesh			*obj1	=	new GraphicEngine::Mesh();
	try
	{
		engine.SetCameraSettings(70.0, 800/600.0, 0.001, 1000);
		engine.SetCameraLocation(glm::vec3(0,0,-100), glm::vec3(0,0,0), glm::vec3(0,1,0));
		obj1->LoadFromFile("../data/cobra.md2");

		unsigned	int	obj1ID	=	0;

		engine.AddObject(obj1,obj1ID);
		// maybe we should encapsulate timer 
		//unsigned int start	=	SDL_GetTicks();
		unsigned int frametime	=	16;
		unsigned int elapsed	=	0;
		while (SDL_GetTicks() < 5000)
		{
			unsigned int begin = SDL_GetTicks();
			// do graphical stuff
			//engine.Clear();
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
	delete	obj1;

	return EXIT_SUCCESS;
}

