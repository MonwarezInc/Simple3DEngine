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
#ifndef MESH_INCLUED
#define MESH_INCLUED
#include "Object.hpp"
#include "../tools/Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/postprocess.h>
#include <vector>
namespace GraphicEngine
{
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
	class Mesh : public IObject
	{
		public:
	
			Mesh();
			virtual	~Mesh();
			virtual	void	LoadFromFile(std::string const & filename);
			virtual	void	Draw(unsigned int elapsed_time, int start , int end);
		private:
			#define NUM_BONES_PER_VERTEX 4
			void	Clear();
			bool	InitFromScene(const aiScene* pScene, std::string const & filename);
			void	InitMesh(unsigned int index, const aiMesh* paiMesh);
			bool	InitMaterials(const aiScene* pScene, std::string const & filename);

			#define INVALID_MATERIAL	0xFFFFFFFF
			struct	MeshEntry
			{
				MeshEntry();
				~MeshEntry();

				void	Init(std::vector<Vertex> const & vertices, std::vector<unsigned int> const & indices);
				GLuint			VAO;
				GLuint			VB;
				GLuint			IB;
				unsigned int	NumIndices;
				unsigned int	MaterialIndex;
			};
			struct 	VertexBoneData
			{
				uint			IDs[NUM_BONES_PER_VERTEX];
				float			Weights[NUM_BONES_PER_VERTEX];
			};
			std::vector<MeshEntry>	m_Entries;
			std::vector<Texture*>	m_Textures;
			const aiScene*			m_pScene;
			Assimp::Importer		m_Importer;
			aiMatrix4x4				m_GlobalInverseTransform;
	};
}
#endif
