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
#include <S3DE_Mesh.h>
#include <cassert>
#include <iostream>
#include <sstream>
#include <glm/gtc/matrix_inverse.hpp>
#define	POSITION_LOCATION		0
#define TEX_COORD_LOCATION		2
#define NORMAL_LOCATION			3
#define BONE_ID_LOCATION		4
#define BONE_WEIGHT_LOCATION	5

// Defining it in a general header
#define BUFFER_OFFSET(i)	reinterpret_cast<char *>(i)

using namespace S3DE;
Mesh::Mesh()
{
	m_pScene	=	nullptr;
	m_NumBones	=	0;
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
										aiProcess_GenSmoothNormals	|	aiProcess_FlipUVs |
										aiProcess_LimitBoneWeights | aiProcess_SplitByBoneCount |
										aiProcess_SplitLargeMeshes);
	if (m_pScene != nullptr)
	{
		m_GlobalInverseTransform	=	aiMatrixToMat4(m_pScene->mRootNode->mTransformation);
		m_GlobalInverseTransform	=	glm::inverse( m_GlobalInverseTransform);
		
		// can launch an except
		if(!this->InitFromScene(m_pScene,filename))
		{
			std::stringstream out;
			out << "error init from scene at: " << __FILE__ << "( " << __LINE__ << ")";
			throw out.str();
		}
	}
	else
		throw std::string("Error parsing" + filename + " : " + m_Importer.GetErrorString() );

}
bool	Mesh::InitFromScene(const aiScene* pScene, std::string const & filename)
{
	m_Entries.resize(pScene->mNumMeshes);
	m_Textures.resize(pScene->mNumMaterials);
	
	// Initialize the meshes in the scene one by one
	for (unsigned int i = 0; i < m_Entries.size(); ++i)
	{
		auto paiMesh	=	pScene->mMeshes[i];
		this->InitMesh(i, paiMesh, 0, 0); // 0 because it's one VAO by meshEntrie
	}

	return	InitMaterials(pScene, filename);
}		
void	Mesh::Draw(unsigned int  elapsed_time,const Shader &shader, std::string const & animation)
{
	std::vector<glm::mat4>	transforms;
	this->BoneTransform((static_cast<float>(elapsed_time))/1000.0f, transforms, GetAnimationIndex(animation));
	for (unsigned int i=0; i < transforms.size(); ++i)
	{
		// we have to send unform matrix
		std::stringstream	out;
		out << "Bones[" << i << "]";
		GLuint location	=	shader.GetUniformLocation(out.str().c_str());
		glUniformMatrix4fv(location, 1, GL_FALSE, (const GLfloat*) glm::value_ptr(transforms[i]));
	}
	for (auto &entries:  m_Entries)
	{
		glBindVertexArray(entries.VAO);
			auto 	materialIndex	=	entries.MaterialIndex;
			glUniform1i(shader.GetUniformLocation("skinned"),entries.skinned);
			if (materialIndex < m_Textures.size())
			{
				glBindTexture(GL_TEXTURE_2D, m_Textures[materialIndex].GetID());
			}
			glDrawElements(GL_TRIANGLES, entries.NumIndices, GL_UNSIGNED_INT, 0);
			
	}
	glBindVertexArray(0); // OpenGL state machine
 
}
void	Mesh::Clear()
{
	m_pScene	=	nullptr;
	m_NumBones	=	0;
	m_Entries.clear();
	m_Textures.clear();
}	
void	Mesh::InitMesh(unsigned int index, const aiMesh* paiMesh, unsigned int BaseVertex, unsigned int BaseIndex)
{
	m_Entries[index].MaterialIndex	=	paiMesh->mMaterialIndex;
	m_Entries[index].NumIndices		=	paiMesh->mNumFaces * 3;
	m_Entries[index].BaseVertex		=	BaseVertex;
	m_Entries[index].BaseIndex		=	BaseIndex;
	
	// if skinned	
	m_Entries[index].skinned		= (paiMesh->mNumBones > 0)?1:0;
	
	std::vector<Vertex>				vertices;
	std::vector<unsigned int>		indices;
	std::vector<VertexBoneData>		bones;

	unsigned	int				NumVertices	=	0;
	unsigned	int				NumIndices	=	0;

	NumVertices								=	paiMesh->mNumVertices;
	NumIndices								=	3*paiMesh->mNumFaces;
	
	vertices.reserve(NumVertices);
	bones.resize(NumVertices);
	indices.reserve(NumIndices);
	aiVector3D	const			zero3D(0.0f,0.0f,0.0f);
	
	for (unsigned int i = 0; i < paiMesh->mNumVertices; ++i)
	{
		// set vertex information
		auto	pPos		=	&(paiMesh->mVertices[i]);
		auto	pNormal		=	&(paiMesh->mNormals[i]);
		auto	pTexCoord	=	paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]):
													&zero3D;
		Vertex	v(glm::vec3(pPos->x, pPos->y,pPos->z),glm::vec2(pTexCoord->x,pTexCoord->y),
					glm::vec3(pNormal->x,pNormal->y,pNormal->z));

		vertices.push_back(v);
	}
	for (unsigned int i = 0; i < paiMesh->mNumFaces; ++i)
	{
		// set indice information
		auto	face	=	paiMesh->mFaces[i];
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
		auto	pMaterial	=	pScene->mMaterials[i];
		
		
		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString	path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE,0,&path,NULL,NULL,NULL,NULL,NULL) == AI_SUCCESS)
			{
				std::string		fullPath = dir +"/"+path.data;
				
				m_Textures[i].SetFilename(fullPath);
				
				if (!m_Textures[i].Load())
				{
					// maybe use except mechanism ?
					std::cerr << "Error loading texture " << fullPath << std::endl;
					std::cerr << "in file: " <<__FILE__ <<"("<<__LINE__<<")" << std::endl;
					ret				=	false;
				}
			}
		}
		// load a white texture in case the model didn't have it's own texture
		else
		{
			m_Textures[i].SetFilename("./white.png");
			ret	=	m_Textures[i].Load();
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
			glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
			
			glVertexAttribPointer(POSITION_LOCATION,3,GL_FLOAT, GL_FALSE,sizeof(Vertex), 0);
			glVertexAttribPointer(TEX_COORD_LOCATION,2,GL_FLOAT, GL_FALSE,sizeof(Vertex), BUFFER_OFFSET(12));
			glVertexAttribPointer(NORMAL_LOCATION,3,GL_FLOAT, GL_FALSE,sizeof(Vertex), BUFFER_OFFSET(20));

			glEnableVertexAttribArray(POSITION_LOCATION);
			glEnableVertexAttribArray(TEX_COORD_LOCATION);
			glEnableVertexAttribArray(NORMAL_LOCATION);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IB);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * NumIndices, indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, BONES);
			glBufferData(GL_ARRAY_BUFFER, sizeof(bones[0]) * bones.size(), bones.data(), GL_STATIC_DRAW);

			
			glVertexAttribIPointer(BONE_ID_LOCATION, 4, GL_INT, sizeof(VertexBoneData), BUFFER_OFFSET(0));
			glVertexAttribPointer(BONE_WEIGHT_LOCATION, 4 , GL_FLOAT, GL_FALSE, sizeof(VertexBoneData), BUFFER_OFFSET(16));

			glEnableVertexAttribArray(BONE_ID_LOCATION);
			glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glBindVertexArray(0);
}
void	Mesh::LoadBones(unsigned int index, const aiMesh* pMesh, std::vector<VertexBoneData> & bones)
{
	assert(pMesh != nullptr);
	for (unsigned int i=0; i < pMesh->mNumBones; ++i)
	{
		// Assert
		assert(pMesh->mBones[i]	!=	nullptr);
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
		m_BoneMapping[BoneName]				=	BoneIndex;
		// We need to check if BoneIndex is < m_BoneInfo.size()
		// Which can occur some times
		if (BoneIndex	>= m_BoneInfo.size())
		{
			std::stringstream out;
			out << "Error BoneIndex is: " << BoneIndex << " but m_BoneInfo have " << m_BoneInfo.size() << " elements";
			out << "\n" << "Error in "<< __FILE__ << "(" << __LINE__<< ")";
			throw out.str();
		}
		m_BoneInfo[BoneIndex].BoneOffset	=	aiMatrixToMat4(pMesh->mBones[i]->mOffsetMatrix);

		for (unsigned int j=0; j < pMesh->mBones[i]->mNumWeights; ++j)
		{
			unsigned int 	VertexID	=	m_Entries[index].BaseVertex + pMesh->mBones[i]->mWeights[j].mVertexId;
			float			Weight		=	pMesh->mBones[i]->mWeights[j].mWeight;
			if (VertexID < bones.size())
				bones[VertexID].AddBoneData(BoneIndex, Weight);
			else
			{
				std::stringstream out;
				out << "Error bones have " << bones.size() << " elements , but VertexID is equal to " << VertexID;
				out << "\n" << "Error in " << __FILE__ << " (" << __LINE__ << ")" ;
				throw out.str();
			}
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
	std::stringstream out;
	out << "Error we have more than " << NUM_BONES_PER_VERTEX << " Bones";
	throw out.str();
}
void	Mesh::BoneTransform(float TimeInSec, std::vector<glm::mat4> & Transforms, unsigned int idAnimation)
{
	if (!m_pScene->HasAnimations()) // if we have no animation quit the boneTransform
		return;
	glm::mat4	Identity		=	glm::mat4();
	auto	TicksPerSec		=	(m_pScene->mAnimations[idAnimation] != nullptr )? 
									m_pScene->mAnimations[idAnimation]->mTicksPerSecond : static_cast<float>(25.0);
	
	auto	TimeInTicks		=	TimeInSec * TicksPerSec;
	auto	AnimationTime	=	fmod(TimeInTicks, m_pScene->mAnimations[idAnimation]->mDuration);

	this->ReadNodeHiearchy(AnimationTime, m_pScene->mRootNode, Identity, idAnimation);

	Transforms.resize(m_NumBones);
	for (unsigned int i=0; i < m_NumBones; ++i)
	{
		Transforms[i]	=	m_BoneInfo[i].FinalTransformation;
	}
}
void	Mesh::ReadNodeHiearchy(float AnimationTime, const aiNode* pNode, glm::mat4 const & ParentTransform, unsigned int idAnimation)
{
	std::string 		NodeName(pNode->mName.data);
	
	auto	pAnimation			=	m_pScene->mAnimations[idAnimation];
	glm::mat4			NodeTransformation;
	NodeTransformation						=	aiMatrixToMat4(pNode->mTransformation);
	
	auto	pNodeAnim			=	this->FindNodeAnim(pAnimation, NodeName);
	
	if (pNodeAnim != nullptr)
	{
		// Interpolate Scaling
		aiVector3D		scaling;
		this->CalcInterpolatedScaling(scaling, AnimationTime, pNodeAnim);
		glm::mat4		scalingM;
		scalingM		=	glm::scale(scalingM,glm::vec3(scaling.x,scaling.y,scaling.z));
		// Interpolate rotation
		aiQuaternion	rotationQ;
		this->CalcInterpolatedRotation(rotationQ, AnimationTime, pNodeAnim);
		glm::mat4	rotationM;
		rotationM		=	aiMatrixToMat4(rotationQ.GetMatrix());
		// Interpolate translation
		aiVector3D		translation;
		this->CalcInterpolatedPosition(translation, AnimationTime, pNodeAnim);
		glm::mat4	translationM;
		translationM	=	glm::translate(translationM,glm::vec3(translation.x,translation.y,translation.z));
		
		// Combine above transformation
		NodeTransformation	=	translationM * rotationM * scalingM;

	}
	glm::mat4 GlobalTransformation	=	ParentTransform * NodeTransformation;
	
	if (m_BoneMapping.find(NodeName) != m_BoneMapping.end())
	{
		unsigned int	BoneIndex	=	m_BoneMapping[NodeName];
		m_BoneInfo[BoneIndex].FinalTransformation	=	m_GlobalInverseTransform * GlobalTransformation * m_BoneInfo[BoneIndex].BoneOffset;
	}

	for (unsigned int i= 0; i < pNode->mNumChildren; ++i)
	{
		this->ReadNodeHiearchy(AnimationTime, pNode->mChildren[i], GlobalTransformation, idAnimation);
	}
}	

unsigned int 		Mesh::FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys -1 ; ++i)
	{
		if (AnimationTime < static_cast<float> (pNodeAnim->mScalingKeys[i+1].mTime))
		{
			return i;
		}
	}
	
	assert(0);

	return 0;
}
unsigned int	Mesh::FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumRotationKeys > 0);
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; ++i)
	{
		if (AnimationTime < static_cast<float> (pNodeAnim->mRotationKeys[i+1].mTime))
			return i;
	}
	
	assert(0);
	return 0;
}
unsigned int	Mesh::FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumPositionKeys > 0);
	for (unsigned int i=0; i < pNodeAnim->mNumPositionKeys -1; ++i)
	{
		if (AnimationTime < static_cast<float> (pNodeAnim->mPositionKeys[i+1].mTime))
			return i;
	}
	assert(0);
	return 0;
}
void	Mesh::CalcInterpolatedScaling(aiVector3D & Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumScalingKeys == 1)
	{
		Out	=	pNodeAnim->mScalingKeys[0].mValue;
		return;
	}
	
	auto	ScalingIndex		=	this->FindScaling(AnimationTime,pNodeAnim);
	auto	NextScalingIndex	=	ScalingIndex + 1;

	assert(NextScalingIndex < pNodeAnim->mNumScalingKeys);

	auto	DeltaTime			=	static_cast<float> (pNodeAnim->mScalingKeys[NextScalingIndex].mTime - 
											pNodeAnim->mScalingKeys[ScalingIndex].mTime);
	auto	Factor				=	AnimationTime - static_cast<float>(pNodeAnim->mScalingKeys[ScalingIndex].mTime/DeltaTime);
	
	this->CheckFactor(Factor, __FILE__, __LINE__);

	const auto&	Start			=	pNodeAnim->mScalingKeys[ScalingIndex].mValue;
	const auto&	End				=	pNodeAnim->mScalingKeys[NextScalingIndex].mValue;
	auto	Delta			=	End - Start;
	Out 								=	Start + Factor * Delta;	
}
void	Mesh::CalcInterpolatedRotation(aiQuaternion & Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		Out	=	pNodeAnim->mRotationKeys[0].mValue;
		return;
	}
	
	auto	RotationIndex		=	this->FindRotation(AnimationTime,pNodeAnim);
	auto	NextRotationIndex	=	RotationIndex + 1;

	assert(NextRotationIndex < pNodeAnim->mNumRotationKeys);

	auto	DeltaTime			=	static_cast<float>(pNodeAnim->mRotationKeys[NextRotationIndex].mTime - 
											pNodeAnim->mRotationKeys[RotationIndex].mTime);
	auto	Factor				=	AnimationTime - static_cast<float>(pNodeAnim->mRotationKeys[RotationIndex].mTime/DeltaTime);
	
	this->CheckFactor(Factor, __FILE__, __LINE__);

	const auto&	Start			=	pNodeAnim->mRotationKeys[RotationIndex].mValue;
	const auto&	End				=	pNodeAnim->mRotationKeys[NextRotationIndex].mValue;
	aiQuaternion::Interpolate(Out, Start, End, Factor);
	Out.Normalize();
}
void	Mesh::CalcInterpolatedPosition(aiVector3D & Out, float AnimationTime, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		Out	=	pNodeAnim->mPositionKeys[0].mValue;
		return;
	}
	
	auto	PositionIndex		=	this->FindPosition(AnimationTime,pNodeAnim);
	auto	NextPositionIndex	=	PositionIndex + 1;

	assert(NextPositionIndex < pNodeAnim->mNumPositionKeys);

	auto	DeltaTime			=	static_cast<float>(pNodeAnim->mPositionKeys[NextPositionIndex].mTime - 
											pNodeAnim->mPositionKeys[PositionIndex].mTime);
	auto	Factor				=	AnimationTime - static_cast<float>(pNodeAnim->mPositionKeys[PositionIndex].mTime/DeltaTime);
		
	this->CheckFactor(Factor, __FILE__, __LINE__);

	const auto& 	Start			=	pNodeAnim->mPositionKeys[PositionIndex].mValue;
	const auto& 	End				=	pNodeAnim->mPositionKeys[NextPositionIndex].mValue;
	aiVector3D			Delta			=	End - Start;
	Out 								=	Start + Factor * Delta;	
}
const aiNodeAnim*	Mesh::FindNodeAnim(const aiAnimation* pAnimation, std::string const & NodeName)
{
	for (unsigned int i=0; i < pAnimation->mNumChannels; ++i)
	{
		const auto	pNodeAnim	=	pAnimation->mChannels[i];
		if (pNodeAnim != nullptr)
		{
			if (std::string(pNodeAnim->mNodeName.data) == NodeName)
				return pNodeAnim;
		}
	}
	
	return nullptr;
}
unsigned int 		Mesh::GetAnimationIndex(std::string const & animation)
{
	unsigned int idAnimation	=	0;
	// Some default case when there is no animation or when m_pScene is nullptr
	if (m_pScene == nullptr)
		return idAnimation;
	if (!m_pScene->HasAnimations())
		return idAnimation;
	//  Arbitrary set the different value for animation
	if (animation == "walk")
		idAnimation	=	1;
	// Then check if the idAnimation is in a good range
	if (idAnimation >= m_pScene->mNumAnimations)
	{
		std::cerr << "Error in idAnimation range" << std::endl;
		std::stringstream out;
		out << "Error in idAnimation range, there is only: " << m_pScene->mNumAnimations;
		out << " but the following id animation has request " << idAnimation << std::endl;
		throw out.str();
	}
	return idAnimation;
}
inline glm::mat4 	S3DE::aiMatrixToMat4(aiMatrix4x4 const & src)
{
	glm::mat4 	dest;
	dest[0][0]	=	src.a1;	dest[0][1]	=	src.b1;	dest[0][2]	=	src.c1;	dest[0][3]	=	src.d1;	
	dest[1][0]	=	src.a2;	dest[1][1]	=	src.b2;	dest[1][2]	=	src.c2;	dest[1][3]	=	src.d2;	
	dest[2][0]	=	src.a3;	dest[2][1]	=	src.b3;	dest[2][2]	=	src.c3;	dest[2][3]	=	src.d3;	
	dest[3][0]	=	src.a4;	dest[3][1]	=	src.b4;	dest[3][2]	=	src.c4;	dest[3][3]	=	src.d4;	
	return 		dest;
}
inline glm::mat4 	S3DE::aiMatrixToMat4(aiMatrix3x3 const & src)
{
	glm::mat4 	dest;
	dest[0][0]	=	src.a1;	dest[0][1]	=	src.b1;	dest[0][2]	=	src.c1;	dest[0][3]	=	0;	
	dest[1][0]	=	src.a2;	dest[1][1]	=	src.b2;	dest[1][2]	=	src.c2;	dest[1][3]	=	0;	
	dest[2][0]	=	src.a3;	dest[2][1]	=	src.b3;	dest[2][2]	=	src.c3;	dest[2][3]	=	0;	
	dest[3][0]	=	0	;	dest[3][1]	=	0	;	dest[3][2]	=	0	;	dest[3][3]	=	1;	
	return 		dest;
}
void 				Mesh::CheckFactor(float Factor, std::string const &file, int line)
{
	if (Factor < 0.0f || Factor > 1.0f)
	{
		std::stringstream out;
		out << "Error interpolated factor not in the good range , Factor is: " << Factor << " which is not in [0.0, 1.0] \n";
		out << "This happend in " << file << "(" << line << ")";
		throw out.str();
	}
}
