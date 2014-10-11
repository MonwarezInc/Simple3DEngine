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
#include <assert.h>
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
		if(!this->InitFromScene(pScene,filename))
			throw std::string("error init from scene");
	}
	else
		throw std::string("Error parsing" + filename + " : " + importer.GetErrorString() );
}
bool	Mesh::InitFromScene(const aiScene* pScene, std::string const & filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);
	
	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); ++i)
	{
		const aiMesh*	paiMesh	=	pScene->mMeshes[i];
		this->InitMesh(i, paiMesh);
	}

	return	InitMaterials(pScene, filename);
}		
void	Mesh::Draw(unsigned int elapsed_time, int start , int end)
{
	// not implemented yet
}
void	Mesh::Clear()
{
	// not implemented yet
}	
void	Mesh::InitMesh(unsigned int index, const aiMesh* paiMesh)
{
	m_Entries[index].MaterialIndex	=	paiMesh->mMaterialIndex;
	std::vector<Vertex>			vertices;
	std::vector<unsigned int>	indices;
	
	aiVector3D	const			zero3D(0.0f,0.0f,0.0f);
	
	for (unsigned int i = 0; i < paiMesh->mNumVertices; ++i)
	{
		// set vertex information
		const 	aiVector3D*	pPos		=	&(paiMesh->mVertices[i]);
		const	aiVector3D*	pNormal		=	&(paiMesh->mNormals[i]);
		const	aiVector3D*	pTexCoord	=	paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]):
													&zero3D;
		Vertex	v(glm::vec3(pPos->x, pPos->y,pPos->z),glm::vec2(pTexCoord->x,pTexCoord->y),
					glm::vec3(pNormal->x,pNormal->y,pNormal->z));

		vertices.push_back(v);
	}
	for (unsigned int i = 0; i < paiMesh->mNumFaces; ++i)
	{
		// set indice information
		const	aiFace& face	=	paiMesh->mFaces[i];
		assert(face.mNumIndices	==	3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	m_Entries[index].Init(vertices,indices);
}
		
bool	Mesh::InitMaterials(const aiScene* pScene, std::string const & filename)
{
	// not implemented yet
	return false;
}
Mesh::MeshEntry::~MeshEntry()
{
	// not implemented yet
}
Mesh::MeshEntry::MeshEntry()
{
	// not implemented yet
}
void	Mesh::MeshEntry::Init(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices)
{
	// not implemented yet
}
