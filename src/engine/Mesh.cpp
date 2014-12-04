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
}
Mesh::~Mesh()
{
	this->Clear();
}
void	Mesh::LoadFromFile(std::string const & filename)
{
	// release the previously loaded mesh (if exists)
	this->Clear();
	
	Assimp::Importer importer;
	m_pScene	=	m_Importer.ReadFile(filename.c_str(), aiProcess_Triangulate |
									aiProcess_GenSmoothNormals	|	aiProcess_FlipUVs);
	if (m_pScene)
	{
		m_GlobalInverseTransform	=	m_pScene->mRootNode->mTransformation;
		m_GlobalInverseTransform.Inverse();
		// can launch an except
		if(!this->InitFromScene(m_pScene,filename))
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
	// animation not implemented yet
	(void)elapsed_time;
	(void)start;
	(void)end;
	/* So
		0 is for vertex position
		2 is for texture coordinate
		3 is for normal
	*/

	for (unsigned int i=0; i < m_Entries.size(); ++i)
	{
		glBindVertexArray(m_Entries[i].VAO);
			const unsigned int materialIndex	=	m_Entries[i].MaterialIndex;
			
			if (materialIndex < m_Textures.size() && m_Textures[materialIndex] )
			{
				glBindTexture(GL_TEXTURE_2D, m_Textures[materialIndex]->GetID());
			}
			glDrawElements(GL_TRIANGLES, m_Entries[i].NumIndices, GL_UNSIGNED_INT, 0);
			
	}
	glBindVertexArray(0); // OpenGL state machine
 
}
void	Mesh::Clear()
{
	for (unsigned int i = 0; i < m_Textures.size() ; ++i)
	{
		delete 	m_Textures[i];
	}
}	
void	Mesh::InitMesh(unsigned int index, const aiMesh* paiMesh)
{
	m_Entries[index].MaterialIndex	=	paiMesh->mMaterialIndex;
	std::vector<Vertex>				vertices;
	std::vector<unsigned int>		indices;
	std::vector<VertexBoneData>		bones;


	unsigned	int				NumVertices	=	0;
	unsigned	int				NumIndices	=	0;

	NumVertices								=	paiMesh->mNumVertices;
	NumIndices								=	3*paiMesh->mNumFaces;

	vertices.reserve(NumVertices);
	bones.reserve(NumVertices);
	indices.resize(NumIndices);
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
	this->LoadBones(index, paiMesh, bones);
	m_Entries[index].Init(vertices,indices,bones);
}
		
bool	Mesh::InitMaterials(const aiScene* pScene, std::string const & filename)
{
	// extract the directory part from the filename
	std::string::size_type	slashIndex	=	filename.find_last_of("/");
	std::string				dir;
	if (slashIndex	==	std::string::npos)
		dir = 	".";
	else if (slashIndex	==	0)
		dir	=	"/";
	else
		dir	=	filename.substr(0,slashIndex);
	
	bool ret	=	true;
	
	// initialize the materials
	for (unsigned int i = 0; i < pScene->mNumMaterials; ++i)
	{
		const	aiMaterial*	pMaterial	=	pScene->mMaterials[i];
		
		m_Textures[i]	=	NULL;
		
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString	path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE,0,&path,NULL,NULL,NULL,NULL,NULL) == AI_SUCCESS)
			{
				std::string		fullPath = dir +"/"+path.data;
				
				m_Textures[i]	=	new Texture(fullPath);
				
				if (!m_Textures[i]->Load())
				{
					// maybe use except mechanism ?
					std::cerr << "Error loading texture " << fullPath << std::endl;
					delete	m_Textures[i];
					m_Textures[i]	=	NULL;
					ret				=	false;
				}
			}
		}
		
		// load a white texture in case the model didn't have it's own texture
		if (!m_Textures[i])
		{
			m_Textures[i]	=	new	Texture("./white.png");
			ret				=	m_Textures[i]->Load();
		}
	}	
		
	return ret;
}
Mesh::MeshEntry::~MeshEntry()
{
	if (0 != VB)
		glDeleteBuffers(1,&VB);
	if (0 != IB)
		glDeleteBuffers(1,&IB);
	if (0 != VAO)
		glDeleteVertexArrays(1,&VAO);
	if (0 != BONES)
		glDeleteVertexArrays(1,&BONES);
}
Mesh::MeshEntry::MeshEntry()
{
	VB				=	0;
	IB				=	0;
	VAO				=	0;
	BONES			=	0;
	NumIndices		=	0;
	MaterialIndex	=	INVALID_MATERIAL;
}
void	Mesh::MeshEntry::Init(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices, 
								std::vector<VertexBoneData> const & bones)
{
	NumIndices		=	indices.size();
	glGenBuffers(1,&VB);
	glGenBuffers(1,&IB);
	glGenBuffers(1,&BONES);
	glGenVertexArrays(1,&VAO);
	
	
	// Vertex Array Object to OpenGL 3.2 core profile
	glBindVertexArray(VAO);
	
		glBindBuffer(GL_ARRAY_BUFFER, VB);
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
			
			glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE,sizeof(Vertex), 0);
			glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE,sizeof(Vertex), (const GLvoid*)12);
			glVertexAttribPointer(3,3,GL_FLOAT, GL_FALSE,sizeof(Vertex), (const GLvoid*)20);

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(2);
			glEnableVertexAttribArray(3);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, &indices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BONES);
			glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), &bones[0], GL_STATIC_DRAW);
			
			glVertexAttribIPointer(4, 4, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
			glVertexAttribPointer(5, 4 , GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), (const GLvoid*)16);

			glEnableVertexAttribArray(4);
			glEnableVertexAttribArray(5);
	glBindVertexArray(0);
}
void	Mesh::LoadBones(unsigned int index, const aiMesh* pMesh, std::vector<VertexBoneData> & bones)
{
	for (unsigned int i=0; i < pMesh->mNumBones; ++i)
	{
		unsigned int 	BoneIndex	=	0;
		std::string		BoneName(pMesh->mBones[i]->mName.data);
		
		if (m_BoneMapping.find(BoneName)	==	m_BoneMapping.end())
		{
			BoneIndex	=	m_NumBones;
			++m_NumBones;
			BoneInfo	bi;
			m_BoneInfo.push_back(bi);
		}
		else
		{
			BoneIndex	=	m_BoneMapping[BoneName];
		}
		m_BoneMapping[BoneName]	=	BoneIndex;
		m_BoneInfo[BoneIndex].BoneOffset	=	pMesh->mBones[i]->mOffsetMatrix;
		
		for (unsigned int j=0; j < pMesh->mBones[i]->mNumWeights; ++j)
		{
			unsigned int 	VertexID	=	m_Entries[index].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float			Weight		=	pMesh->mBones[i]->mWeights[j].mWeight;
			bones[VertexID].AddBoneData(BoneIndex, Weight);
		}
	}	
}
void	Mesh::VertexBoneData::AddBoneData(unsigned int BoneID, float Weight)
{
	for (unsigned int i=0; i < NUM_BONES_PER_VERTEX; ++i)
	{
		if (Weights[i]	==	0.0)
		{
			IDs[i]		=	BoneID;
			Weights[i]	=	Weight;

			return;
		}
	}
	// if we have more than NUM_BONES_PER_VERTEX
	assert(0);
}
