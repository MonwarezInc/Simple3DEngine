// GLSL version
#version 130

// Input
in vec2 			coordTexture;
in vec3 			Normal0;
// Uniform
uniform sampler2D	text;	 
// Output
out vec4 out_Color;

void main()
{
	out_Color	=	texture2D(text,coordTexture.xy);
}

