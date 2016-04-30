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
#ifndef MESH_INCLUED
#define MESH_INCLUED
#include "S3DE_Texture.h"
#include "S3DE_Shader.h"

#include <Importer.hpp>
#include <scene.h>
#include <mesh.h>
#include <postprocess.h>
#include <vector>
#include <map>
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace S3DE
{
inline	glm::mat4	aiMatrixToMat4(aiMatrix4x4 const & src);
inline	glm::mat4	aiMatrixToMat4(aiMatrix3x3 const & src);
struct	Vertex
{
	glm::vec3	m_pos;
	glm::vec2	m_tex;
	glm::vec3	m_normal;
	
	Vertex() {}
	
	Vertex(glm::vec3 const & pos, glm::vec2 const & tex, glm::vec3 const & normal)
	{
		m_pos	=	pos;
		m_tex	=	tex;
		m_normal=	normal;
	}
};
class Mesh
{
	public:

		Mesh();
		virtual	~Mesh();
		///	\brief	Load a mesh from a file
		///
		///	This function can launch exception and assertion, the loader are really strict. <br/>
		///	Loading a resource that is actually loaded may lead to unbehavior issue.
		///	\param	filename	filename of the Mesh to load
		virtual	void	LoadFromFile(std::string const & filename);
		///	\brief	Draw the mesh with the animation specified WIP
		///	
		///	For the moment only the first animation is supported, need some resource with multiple animation <br/>
		///	which can be loaded, and need to implement a correct search animation id by name function
		///	\param	elapsed_time	The elapsed time since the beginning of the application
		///	\param	shader			A reference to set some information to the shader (like bone information etc...)
		///	\param	animation		The name of the animation to play
		virtual	void	Draw(unsigned int  elapsed_time, Shader const & shader,std::string const & animation);
	private:
		#define NUM_BONES_PER_VERTEX 4
		#define INVALID_MATERIAL	0xFFFFFFFF
		
		struct 	BoneInfo
		{
			glm::mat4		BoneOffset;
			glm::mat4		FinalTransformation;

			BoneInfo()
			{
				BoneOffset			=	glm::mat4(0);
				FinalTransformation	=	glm::mat4(0);
			}
		};
		struct 	VertexBoneData
		{
			uint			IDs[NUM_BONES_PER_VERTEX];
			float			Weights[NUM_BONES_PER_VERTEX];
			
			VertexBoneData()
			{
				for (unsigned int i=0; i < NUM_BONES_PER_VERTEX; ++i)
				{
					IDs[i]		=	0;
					Weights[i]	=	0;
				}
			}
			void			AddBoneData(unsigned int boneID, float weight);
		};
		void		Clear();
		bool		InitFromScene(const aiScene* pScene, std::string const & filename);
		void		InitMesh(unsigned int index, const aiMesh* paiMesh, unsigned int BaseVertex, unsigned int BaseIndex);
		bool		InitMaterials(const aiScene* pScene, std::string const & filename);
		void		LoadBones(unsigned int index, const aiMesh* , std::vector<VertexBoneData> & bones);			
		void		BoneTransform(float TimeInSec, std::vector<glm::mat4>& Transforms, unsigned int idAnimation);
		void		ReadNodeHiearchy(float AnimationTime, const aiNode* pNode, glm::mat4 const & ParentTransform, unsigned int idAnimation);

		void		CalcInterpolatedScaling(aiVector3D & Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void		CalcInterpolatedRotation(aiQuaternion & Out, float AnimationTime, const aiNodeAnim* pNodeAnim);
		void		CalcInterpolatedPosition(aiVector3D & Out, float AnimationTime, const aiNodeAnim* pNodeAnim);

		const aiNodeAnim*	FindNodeAnim(const aiAnimation* pAnimation, std::string const & NodeName);
		unsigned int		FindScaling(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int		FindRotation(float AnimationTime, const aiNodeAnim* pNodeAnim);
		unsigned int		FindPosition(float AnimationTime, const aiNodeAnim* pNodeAnim);

		unsigned int		GetAnimationIndex(std::string const & animation);
		void		CheckFactor(float Factor, std::string const & file, int line);
		struct	MeshEntry
		{
			MeshEntry();
			~MeshEntry();

			void	Init(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices, 
							std::vector<VertexBoneData> const & bones);
			GLuint			VAO;
			GLuint			VB;
			GLuint			IB;
			GLuint			BONES;
			unsigned int	NumIndices;
			unsigned int	MaterialIndex;
			unsigned int	BaseVertex;
			unsigned int	BaseIndex;
			int				skinned;
		};
		std::vector<MeshEntry>				m_Entries;
		std::vector<Texture>				m_Textures;
		const aiScene*						m_pScene;
		Assimp::Importer					m_Importer;
		glm::mat4							m_GlobalInverseTransform;
		std::map<std::string, unsigned int>	m_BoneMapping;
		unsigned int						m_NumBones;
		std::vector<BoneInfo>				m_BoneInfo;
};
}  // end of S3DE namespace
#endif
