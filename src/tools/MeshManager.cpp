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
#include <S3DE_MeshManager.h>

using namespace S3DE;
#define NULL_RC	0
MeshManager::MeshManager()
{
	// Add an empty rc
	RcField	rc	=	{std::string(""), NULL_RC};
	m_rcfield.push_back(rc);
	m_count.push_back(0);
}
MeshManager::~MeshManager()
{
}
RcField	MeshManager::Load(std::string const &filename)
{
	/*	So what happend: 	m_count: (0 1 2 1 3 5 2 1 3)
	*						m_rcfield: (NULL_RC rc1 rc2 rc3 ...)
	*						m_pmesh: (p1 p2 p3 ...)
	*
	*	The link ?: m_count[i] refer to the count of m_pmesh[i-1] if i > 0
	*/
	RcField	rc	= {filename, NULL_RC};
	if (filename == "")
		throw std::string ("Can't load a Mesh with empty name");

	auto	n	=	m_rcfield.size();
	bool	foundempty	=	false;
	size_t	emptyind	=	0;

	size_t	i	=	0;
	for (i = 1; i < n; ++i)
	{
		if (filename == m_rcfield[i].filename)
			break;
		if (!foundempty)
		{
			if (m_count[i]	==	0)
			{
				foundempty	=	true;
				emptyind	=	i;
			}
		}
	}
	// This is a new Mesh
	if ( i == n)
	{
		auto	mesh	=	std::make_shared<Mesh>();
		mesh->LoadFromFile(filename);
		if (foundempty)
		{
			m_pmesh[emptyind - 1].push_back(mesh);
			m_count[emptyind]	=	1;
			rc.id	=	emptyind;
			m_rcfield[emptyind]	=	rc;
		}
		else
		{
			std::vector<std::shared_ptr<Mesh> >	mesharray;
			mesharray.push_back(mesh);
			m_pmesh.push_back(mesharray);
			m_count.push_back(1);
			rc.id	=	m_count.size() - 1;
			m_rcfield.push_back(rc);
		}
	}
	else 
	{
		++m_count[i];
		rc	=	m_rcfield[i];
	}
	
	return rc;
}
void	MeshManager::Release(RcField	&rc)
{
	if ((rc.id < m_count.size()) && (rc.id > 0))
	{
		// Release if count == 1
		if (1	==	m_count[rc.id])
		{
			// release the mesh
			m_count[rc.id]	=	0;
			m_pmesh[rc.id - 1].clear();
			m_rcfield[rc.id].filename	=	"";
			m_rcfield[rc.id].id			=	NULL_RC;
		}
		if (m_count[rc.id] > 1)
			m_count[rc.id]--;
	}
	rc.filename=	std::string("");
	rc.id	=	0;
}
void	MeshManager::Draw(RcField const & rc, unsigned int elapsed_time, Shader const & shader, std::string const & animation)
{
	if ((rc.id < m_count.size()) && (rc.id > 0))
		m_pmesh[rc.id - 1][0]->Draw(elapsed_time, shader, animation);
	// Else silently discard the draw 
}
