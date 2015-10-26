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
