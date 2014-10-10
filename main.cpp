#include <iostream>
#include "engine/Engine-main.h"
using namespace std;
int main (int argc, char **argv)
{
	(void) argc;
	(void) argv;

	cout << "Test engine " << endl;
	
	GraphicEngine::CEngine	engine;
	engine.CreateWindow(800,600,false,"Test Engine", 32, 2, 2,1);
	engine.ClearColor(0.0,0.0,0.0,1.0);
	// maybe we should encapsulate timer 
	while (SDL_GetTicks() < 5000)
	{
		// do graphical stuff
		engine.Clear();
		engine.Draw();

	}
	
	return EXIT_SUCCESS;
}

