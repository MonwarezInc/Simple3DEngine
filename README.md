GraphicEngine
=============

GraphicEngine turned to OpenGL 3.2 (core profile) and 330 for shader 

This is a graphics engine for a future project,
I will probably reuse a part of my personal project.<br/>
You can build it with cmake commands<br/>
If u have a different location of libraries<br/>
You can add to CMakeCache.txt (created by the first invoke of cmake) these lines: <br/>
lib_include:FILEPATH=/path/to/include/directory <br/>
lib_lib:FILEPATH=/path/to/lib/directory <br/>
Note: <br/>
Support for mesh with bones seems not working at all, it's result on a seg fault for unknow reason. <br/>
Only one mesh with bones are tested, maybe is the model.
Ok so maybe it's because a premature memory delete.<br/>
In fact Weights and IDs are not nullptr , but we get SIGSEV by trying to see they value<br/>

Copyright (C) 2014-2015 Payet Thibault

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

For Assimp Loader , I use a code in tutorial22 from <http://ogldev.atspace.co.uk/index.html>
