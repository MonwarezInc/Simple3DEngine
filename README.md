Simple3DEngine
=============
Build status:
  https://gitlab.com/MonwarezInc/Simple3DEngine/badges/master/build.svg

Simple3DEngine turned to OpenGL 3.2 (core profile) and 150 for shader <br/>
List of shader extension used:<br/>
GL_ARB_explicit_attrib_location <br/>

This is a graphics engine for a future project,
I will probably reuse a part of my personal project.<br/>
You can build it with cmake commands<br/>
Libraries depends are: GLEW, SDL2,SDL2_image, Assimp <br/>

Feature: skinned animation works. <br/>
In `include/bundle`, this is glm with licence described in `include/bundle/copying.txt` <br/>
Copyright (c) 2016, Payet Thibault<br/>
All rights reserved.<br/>

Redistribution and use in source and binary forms, with or without<br/>
modification, are permitted provided that the following conditions are met:<br/>
    * Redistributions of source code must retain the above copyright<br/>
      notice, this list of conditions and the following disclaimer.<br/>
    * Redistributions in binary form must reproduce the above copyright<br/>
      notice, this list of conditions and the following disclaimer in the <br/>
      documentation and/or other materials provided with the distribution.<br/>
    * Neither the name of the Monwarez Inc nor the<br/>
      names of its contributors may be used to endorse or promote products<br/>
      derived from this software without specific prior written permission.<br/>

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND<br/>
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED<br/>
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE<br/>
DISCLAIMED. IN NO EVENT SHALL PAYET THIBAULT BE LIABLE FOR ANY<br/>
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES<br/>
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;<br/>
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND<br/>
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT<br/>
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS<br/>
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.<br/>

For Assimp Loader , I use a code in tutorial22, tutorial38 from <http://ogldev.atspace.co.uk/index.html>
