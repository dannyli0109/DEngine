#version 330

out vec4 FragColour;

in vec4 Colour;

void main()
{
	FragColour = Colour;
}