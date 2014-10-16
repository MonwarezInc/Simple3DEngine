/*
Copyright (C) 2014 Payet thibault

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
/*	Object.h 
	is an interface for general object
*/
#ifndef IOBJECT_INTERFACE_INCLUED
#define IOBJECT_INTERFACE_INCLUED
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

namespace GraphicEngine
{
	/**
	
	Interface IObject
	with an object we can do 2 things:
	load the model , and draw it
	the position/orientation only depend on the scene graph
	**/
	class IObject
	{
		public:
			IObject(){}
			virtual	void	LoadFromFile(std::string const & filename)=0;
			virtual void	Draw(unsigned int elapsed_time, int start, int end)=0;
	};
}
#endif
