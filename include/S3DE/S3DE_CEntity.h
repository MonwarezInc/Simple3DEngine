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
#ifndef S3DE_CENTITY_INCLUED
#define S3DE_CENTITY_INCLUED
#include "S3DE_Mesh.h"
#include "S3DE_ResourceManager.hpp"
/**
*	\brief	namespace of the engine
*/
namespace S3DE
{
	struct	RcField
	{
		std::string		filename;
		unsigned int	id;
	};
	class CEntity
	{
		public:
			/** \brief Construct a new entity and associate with a rcmanager
			*	\param rcmanager a pointer to a derived class of IResourceManager
			*/	
			CEntity(IResourceManager<RcField> *rcmanager);
			/**	\brief Destructor of a entity, it will decrease the counter associated to the resource
			*/
			virtual	~CEntity();
			/** \brief	Request loading a ressource 
			*	\param	the filename of the ressource to load
			*/
			virtual void Load(std::string const &filename);
			/** \brief	Clear the current resource*/
			virtual	void		Clear();
		protected:
			IResourceManager<RcField>*	m_rcmanager;	///< pointer to a derived class of IResourceManager
			RcField						m_rcField;		///< keep informative data of the resource
		

	};
}
#endif
