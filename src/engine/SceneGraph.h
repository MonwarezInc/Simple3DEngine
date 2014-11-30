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
#ifndef SCENE_GRAPH_INCLUED
#define SCENE_GRAPH_INCLUED
#include "Object.hpp"
#include <vector>
#ifndef GLM_FORCE_RADIANS
	#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace	GraphicEngine
{
	struct	Transformation
	{
		glm::vec3	translate;
		glm::vec3	scale;
		glm::quat	rotate;
	};

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
			struct NodeInfo
			{
				IObject*		obj;
				bool			hide;
				bool			transparent;
				Transformation	transformation;
				unsigned int 	id;
			};
			struct	Node
			{
				NodeInfo			nodeInfo;
				Node*				previous;
				std::vector<Node*>	next;
			};
			class	Tree
			{
				public:
					Tree();
					virtual	~Tree();
					virtual	unsigned	int		AddNodeInfo(NodeInfo const & nodeInfo, unsigned int parent);
				protected:
					virtual	Node*				Detach(unsigned int id);
					virtual	bool				Attach(Node const * node, unsigned int parent);
					Node						m_root;
			};
	};	
}
#endif
