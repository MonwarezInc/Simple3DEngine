/*
Copyright (c) 2016, Payet Thibault
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the Monwarez Inc nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef SCENE_GRAPH_INCLUED
#define SCENE_GRAPH_INCLUED
#include "S3DE_Mesh.h"

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>


namespace	S3DE
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
			unsigned int	AddMesh(Mesh* obj,glm::mat4 const & transf, unsigned int parent=0, 
										bool hide= false, bool trans = false);	
			bool			DeleteObject(unsigned int id);
			Mesh*		ChainTransformation(unsigned int id, glm::mat4 & transf);
		protected:
			struct NodeInfo
			{
				Mesh*			obj;
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
