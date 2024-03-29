#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 ourColor;
out vec2 TexCoord;

uniform vec4 ourPos;
 
void main()
{
	gl_Position = vec4(aPos + ourPos.xyz,1.0);
	ourColor = vec4(aColor,1.0);
	//TexCoord = aTexCoord;
	TexCoord = gl_Position.xy*0.5+0.5;
}