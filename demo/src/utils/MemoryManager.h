/*
Copyright (C) 2014-2015 Payet thibault

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
#ifndef MemoryManager_H_INCLUED
#define MemoryManager_H_INCLUED
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

// BasicMemoryManager
template <class T>
class BasicVectorManager
{
	public:
		BasicVectorManager(){}
		BasicVectorManager(size_t nb)
		{
			m_pVect.resize(nb);
			for (size_t i=0; i < nb; ++i)
				m_pVect[i]	=	std::make_unique<T>();
		}
		virtual	void	Allocate(size_t nb)
		{
			this->Release();
			m_pVect.resize(nb);
			for (size_t i=0; i < nb;++i)
				m_pVect[i]	=	std::make_unique<T>();
		}
		virtual	void	Release()
		{
			m_pVect.clear();
		}
		virtual 		~BasicVectorManager()
		{
			this->Release();
		}
		virtual	T*	GetVectPtr(size_t i)
		{
			return m_pVect[i].get();
		}
	protected:
		std::vector<std::unique_ptr<T>>	m_pVect;
};
#endif
