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
#ifndef FILEMANAGER_HEADER_INCLUED
#define FILEMANAGER_HEADER_INCLUED
#include <memory>
#include <cstdio>
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
#endif
