## Introduction

Simple 3D Engine is a bsd3-clauses library that aims to provide simple render system.<br/>
It use OpenGL version 3.2 + and GLSL 1.50 + in core profile.

##	Supported Platform	
	* FreeBSD 
	* Linux

##	Know Issue
	* MeshManager is not ready , it may crash during Release() (SIGSEGV). <br/>
	Need to do some test with only MeshManager loading Mesh resource. I have only tested with the old loading mesh system <br/>
	and the MeshManager loader system. Since I get error from Assimp destructor , it seems that it is like I can't load twice <br/>
	the same resource.
	It need to be rewritten with std::unique_ptr stuff, but need to see why Travis fails with c++14 <br/>

