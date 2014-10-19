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
#include "SceneGraph.h"

using namespace GraphicEngine;
SceneGraph::SceneGraph()
{
	// not implemented yet
}
SceneGraph::~SceneGraph()
{
	// not implemented yet
}
unsigned int	SceneGraph::AddObject(IObject* obj, glm::mat4 const & transf, unsigned int parent, 
										bool hide, bool trans)
{
	// not implemented yet
	(void)obj;
	(void)transf;
	(void)parent;
	(void)hide;
	(void)trans;
	
	return 0;
}
bool			SceneGraph::DeleteObject(unsigned int id)
{
	// not implemented yet
	(void)id;
	return false;
}
IObject*		SceneGraph::ChainTransformation(unsigned int id, glm::mat4 & transf)
{
	// not implemented yet
	(void)id;
	(void)transf;
	return NULL;
}
