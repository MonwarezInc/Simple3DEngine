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
#ifndef MESH_MANAGER_INCLUED
#define MESH_MANAGER_INCLUED
#include "S3DE_IResourceManager.hpp"
#include "S3DE_Mesh.h"
#include <memory>

namespace S3DE
{
/// \brief Structure that contain all useful information about the resource
struct  RcField
{
	std::string     filename;   ///< the filename of the resource
	size_t          id;         ///< an id to select an entity in particular
};
typedef std::vector < std::vector < std::unique_ptr < Mesh > > > Mesh2DArr; ///< A little trick to delete one element of an array
///	\brief	Manage the mesh resource
class	MeshManager	:	public	IResourceManager<RcField>
{
	public:	
		MeshManager();
		~MeshManager();
		///	\brief Load a mesh only once and return a RcField to manage it
		///	\param	filename filename of the resource to load
		RcField		Load(std::string const &filename);
		///	\brief Decrease the count of the resource of a mesh, and eventually release the resource
		///	\param	object decrease the count of the ressource for object
		void		Release(RcField &object);
		///	\brief	Wrapper to the mesh->Draw(...)
		///	\param	rcfield			The RcField so that more verification could be done
		///	\param	elapsed_time	The	elapsed time since the beginning , so that animation could works
		///	\param	shader			A reference to the shader to use
		///	\param	animation		The name of the animation to play
		void		Draw(RcField const & rcfield, unsigned int elapsed_time, Shader const & shader,
							std::string const & animation);
	protected:
		std::vector<RcField>	m_rcfield; 	///< A vector of the rcfield 
		std::vector<size_t>		m_count; 	///< A vector of counter of ressource
		Mesh2DArr				m_pmesh; 	///< A 2D vector of unique_ptr Mesh
};
}  // end of S3DE namespace
#endif
