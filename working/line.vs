#version 330

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_VertColour;

out vec4 Colour;

uniform mat4 u_ProjectionMatrix;

void main()
{
	Colour = a_VertColour;
	gl_Position = u_ProjectionMatrix * vec4(a_Position, 1);
}