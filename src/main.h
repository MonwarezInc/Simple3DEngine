#include <memory>
#include <stdio.h>
#include <string>

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

