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
	GraphicEngine::Mesh			*obj2	=	new	GraphicEngine::Mesh();
	GraphicEngine::Mesh			*obj3	=	new	GraphicEngine::Mesh();

	try
	{
		engine.SetCameraSettings(70.0, 800/600.0, 0.01, 10000);
		engine.SetCameraLocation(glm::vec3(250,0,20), glm::vec3(0,0,0), glm::vec3(0,0,1));
		obj1->LoadFromFile("../data/cobra.md2");
		obj2->LoadFromFile("../data/palm/Palm N161213.3DS");
		obj3->LoadFromFile("../data/girl/Girl walking N090814.3DS");

		unsigned	int	obj1ID	=	0;
		unsigned	int	obj2ID	=	0;
		unsigned	int	obj3ID	=	0;

		engine.AddObject(obj1,obj1ID);
		engine.AddObject(obj2,obj2ID);
		engine.AddObject(obj3,obj3ID);

		engine.SetObjectPosRot(obj1ID, glm::vec3(-10,0,0), glm::vec3(M_PI/2.0, 0.0, 0));
		engine.SetObjectPosRot(obj2ID, glm::vec3(10,20,0), glm::vec3(0.0,0.0,0.0));
		engine.SetObjectScale(obj2ID, 0.5);
		engine.SetObjectPosRot(obj3ID, glm::vec3(0,-30,0), glm::vec3(0.0,0.0,0.0));

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
	delete	obj1;
	delete 	obj2;
	delete	obj3;
	return EXIT_SUCCESS;
}

