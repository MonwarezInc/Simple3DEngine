/*
Copyright (C) 2016 Payet thibault

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
#ifndef RESSOURCEMANAGER_HPP_INCLUED
#define RESSOURCEMANAGER_HPP_INCLUED
/** \file RessourceManager.hpp
*	\brief	Interface for managing Ressource
*
*	This class is an abstract class, you have to derivate it
*
*/
#include <string>
#include <vector>
#include <map>
namespace S3DE
{
	template<class T>
	class IRessourceManager
	{
		public:
			IRessourceManager();
			virtual ~IressourceManager();
			/** \brief pure virtual load function
			*	@param[in]	filename filename of the ressource to load
			*/
			virtual	T	Load(std::string const &filename)=0;
			/** \brief pure virtual release function
			*	@param[in,out]	object decrease the count of the ressource for object
			*/
			virtual void 	Release(T &object)=0;
	}
}
#endif
