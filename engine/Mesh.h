/*
Copyright (C) 2014 Payet thibault

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
#include "Object.hpp"
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
namespace GraphicEngine
{
	class Mesh : public IObject
	{
		public:
	
			Mesh();
			virtual	~Mesh();
			virtual	void	LoadFromFile(std::string const & filename);
			virtual	void	Draw(unsigned int elapsed_time, int start , int end);
		private:
			void	Clear();
			void	InitFromScene(const aiScene* pScene, std::string const & filename);
	};
}
