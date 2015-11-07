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
#include "FileManager.h"
using namespace std;
FileManager::FileManager()
{
	file	=	nullptr;
}
FileManager::FileManager(string const & filename, string const & mode)
{
	file	=	nullptr;
	file	=	fopen(filename.c_str(),mode.c_str());
}
FILE* FileManager::GetFilePtr()
{
	if (!file)
		throw string("Error I/O ");
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
void 	FileManager::LoadFile(string const & filename, string const & mode)
{
	this->Release();
	file	=	fopen(filename.c_str(), mode.c_str());
}
