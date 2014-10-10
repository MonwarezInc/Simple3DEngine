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
#include "Mesh.h"

using namespace GraphicEngine;
Mesh::Mesh()
{
	// not implemented yet
}
Mesh::~Mesh()
{
	// not implemented yet
}
void	Mesh::LoadFromFile(std::string const & filename)
{
	// release the previously loaded mesh (if exists)
	this->Clear();
	
	Assimp::Importer importer;
	const aiScene*	pScene	=	importer.ReadFile(filename.c_str(), aiProcess_Triangulate |
									aiProcess_GenSmoothNormals	|	aiProcess_FlipUVs);
	if (pScene)
	{
		// can launch an except
		this->InitFromScene(pScene,filename);
	}
	else
		throw std::string("Error parsing" + filename + " : " + importer.GetErrorString() );
}
void	Mesh::InitFromScene(const aiScene* pScene, std::string const & filename)
{
	// not implemented yet	
}		
void	Mesh::Draw(unsigned int elapsed_time, int start , int end)
{
	// not implemented yet
}
void	Mesh::Clear()
{
	// not implemented yet
}	
