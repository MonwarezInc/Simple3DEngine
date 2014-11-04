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
#include "Object.hpp"
#include <vector>

namespace	GraphicEngine
{
	class	SceneGraph
	{
		public:
			SceneGraph();
			~SceneGraph();
			// for beginning we just use mat4 transformation , after we will get quaternion transformation
			unsigned int	AddObject(IObject* obj,glm::mat4 const & transf, unsigned int parent=0, 
										bool hide= false, bool trans = false);	
			bool			DeleteObject(unsigned int id);
			IObject*		ChainTransformation(unsigned int id, glm::mat4 & transf);
		protected:
			struct Node
			{
				IObject*		obj;
				bool			hide;
				bool			transparent;
				glm::mat4		transformation;
				unsigned int 	id;
			};
			// now we have to set list and tree structure
			// or use the stl list
	};	
}
