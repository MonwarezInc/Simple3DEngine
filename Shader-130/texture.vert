// Version of GLSL
#version 130

// Vertex input
in vec3	in_Vertex;
in vec3	in_TexCoord0;
in vec3	in_Normal;

// Uniform 
uniform mat4	MVP;

// Output
out vec3 coordTexture;

void main()
{
	// Final position 
	gl_Position		=	MVP * vec4(in_Vertex, 1.0);
	// Output
	coordTexture	=	in_TexCoord0;
}
