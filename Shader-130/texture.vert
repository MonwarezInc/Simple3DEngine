// GLSL version
#version 130

// vertex input
in 		vec3	in_Vertex;
in		vec2	in_TexCoord0;
in		vec3	in_Normal;

// uniform
uniform	mat4	MVP;

// output
out 	vec2	coordTexture;

void	main()
{
	// Final position
	gl_Position	=	MVP * vec4(in_Vertex, 1.0);
	
	// output
	coordTexture	=	in_TexCoord0;
}

