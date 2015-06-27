#ifndef MAIN_H_INCLUED
#define MAIN_H_INCLUED
#include <memory>
#include <stdio.h>
#include <string>
#include <vector>

//	FileManager
class FileManager
{
	public:
		FileManager();
		FileManager(std::string const & filename, std::string const & mode);
		void	LoadFile(std::string const & filename, std::string const & mode);
		FILE*	GetFilePtr();
		void	Release();
		~FileManager();
	protected:
		FILE*	file;
};
FileManager::FileManager()
{
	file	=	nullptr;
}
FileManager::FileManager(std::string const & filename, std::string const & mode)
{
	file	=	nullptr;
	file	=	fopen(filename.c_str(),mode.c_str());
}
FILE* FileManager::GetFilePtr()
{
	if (!file)
		throw std::string("Error I/O ");
	return file;
}
FileManager::~FileManager()
{
	this->Release();	
}
void	FileManager::Release()
{
	if(file)
		fclose(file);
	file	=	nullptr;
}
void 	FileManager::LoadFile(std::string const & filename, std::string const & mode)
{
	this->Release();
	file	=	fopen(filename.c_str(), mode.c_str());
}
// End FileManager
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
// End BasicMemoryManager
#endif
