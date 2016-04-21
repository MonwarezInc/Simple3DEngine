## Introduction

Simple 3D Engine is a bsd3-clauses library that aims to provide simple render system.<br/>
It use OpenGL version 3.2 + and GLSL 1.50 + in core profile.

##	Supported Platform	
	* FreeBSD 
	* Linux

##	Fixed Bugs
	* MeshManager should work now, the error come from S3DE::Mesh::LoadBones ,some BoneIndex that having really hight value cause the issue. <br/>
	 So now a check is performed , and if it is the case , LoadBones will launch an exception.
