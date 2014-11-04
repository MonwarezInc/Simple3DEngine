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
#ifndef RC_MANAGER_H
#define RC_MANAGER_H
#include "../engine/Object.hpp"

/*
	I dont really want use singleton pattern (maybe in future)
	So RcManager will not be a singleton
	but I will have only one RcManager (for graphical stuff)
*/

namespace GraphicEngine
{
	class RcManager
	{
		public:

			RcManager();
	};
}
	

#endif
