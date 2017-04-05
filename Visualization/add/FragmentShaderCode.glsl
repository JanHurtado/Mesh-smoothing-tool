#version 430

out vec4 outColor;
in vec3 theColor;

void main()
{
	outColor = vec4(theColor,1.0);
}